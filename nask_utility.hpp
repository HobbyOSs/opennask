#include <fstream>
#include <sstream>
#include <algorithm>
#include "ParaTokenizer.hh"
#include "ParaOperator.hh"
#include "ParaExpression.hh"
#include "ParaSymbolTable.hh"
#include "ParaMathLibrary.hh"
#include "nask_defs.hpp"

namespace nask_utility {

     // デフォルトのトークンテーブル
     TParaCxxTokenTable token_table;
     size_t dollar_position; // $

     std::ifstream::pos_type filesize(const char* filename) {
	  std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	  return in.tellg();
     }

     std::vector<std::string> split(const std::string &str, char delim){
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

     bool is_register(TParaCxxTokenTable& token_table, TParaToken& token) {
	  // レジスタ一覧から検索してあれば true
	  auto it = std::find_if(std::begin(REGISTERS), std::end(REGISTERS),
				 [&](const std::string& s)
				 { return token.AsString().find(s) != std::string::npos; });

	  return it != std::end(REGISTERS);
     }

     template <class T> void plus_number_from_code(T& num, char c) {
	  switch(c) {
	  case 'A':
	       num + 0;
	       break;
	  case 'C':
	       num + 1;
	       break;
	  case 'D':
	       num + 2;
	       break;
	  case 'B':
	       num + 3;
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

     void set_nimonic_with_register(const std::string& reg, NIMONIC_INFO* nim_info) {

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
	  } else if (reg == "AX" || reg == "BX" || reg == "CX" || reg == "DX") {
	       // prefix = "66B8+rw" (AX:+0, BX:+1, CX:+2, DX:+3)
	       nim_info->prefix = get_plus_register_code((uint16_t) 0x66b8, reg.at(1));
	       nim_info->reg = reg;
	       nim_info->imm = imm16;
	  } else {

	  }

	  return;
     }

     // uint16_tで数値を読み取った後、uint8_t型にデータを分けて、リトルエンディアンで格納する
     // nask的には0x00をバイトサイズで格納する傾向があるので、そうじゃない場合はフラグを設定する
     void set_word_into_binout(const uint16_t& word, std::vector<uint8_t>& binout_container, bool zero_as_byte = true) {

	  if (word == 0x0000 && zero_as_byte) {
	       // push_back only 1byte
	       binout_container.push_back(0x00);
	  } else {
	       // http://stackoverflow.com/a/1289360/2565527
	       const uint8_t first_byte  = word & 0xff;
	       const uint8_t second_byte = (word >> 8);
	       // push_back as little_endian
	       binout_container.push_back(second_byte);
	       binout_container.push_back(first_byte);
	  }
     }

     // uint32_tで数値を読み取った後、uint8_t型にデータを分けて、リトルエンディアンで格納する
     // nask的には0x00をバイトサイズで格納する傾向があるので、そうじゃない場合はフラグを設定する
     void set_dword_into_binout(const uint32_t& dword, std::vector<uint8_t>& binout_container, bool zero_as_byte = true) {

	  if (dword == 0x00000000 && zero_as_byte) {
	       // push_back only 1byte
	       binout_container.push_back(0x00);
	  } else {
	       uint32_t cp_dword = dword;
	       uint8_t bytes[4];
	       bytes[3] = (uint8_t) cp_dword;
	       bytes[2] = (uint8_t) (cp_dword >>= 8);
	       bytes[1] = (uint8_t) (cp_dword >>= 8);
	       bytes[0] = (uint8_t) (cp_dword >>= 8);

	       // push_back as little_endian
	       for (uint8_t byte : bytes) {
		    binout_container.push_back(byte);
	       }
	  }
     }

     // MOV命令
     // http://www5c.biglobe.ne.jp/~ecb/assembler/2_1.html
     // MOV DEST, SRC
     //     動作：DEST←SRC
     //     DEST：レジスタ、メモリー
     //     SRC ：レジスタ、メモリー、即値（ただしメモリー、メモリーの組み合わせは除く）
     int process_token_MOV(TParaTokenizer& tokenizer, std::vector<uint8_t>& binout_container) {

	  // See: http://d.hatena.ne.jp/yz2cm/20130601/1370078834
	  // インテル記法じゃないので逆なのに注意
	  //
          // b0 [imm8]		mov [imm8],%al
          // b3 [imm8]		mov [imm8],%bl
          // b1 [imm8]		mov [imm8],%cl
          // b2 [imm8]		mov [imm8],%dl
          // b8 [imm32]		mov [imm32],%eax
          // bb [imm32]		mov [imm32],%ebx
          // b9 [imm32]		mov [imm32],%ecx
          // ba [imm32]		mov [imm32],%edx
          // 66 b8 [imm16]	mov [imm16],%ax
          // 66 bb [imm16]	mov [imm16],%bx
          // 66 b9 [imm16]	mov [imm16],%cx
          // 66 ba [imm16]	mov [imm16],%dx
	  //
	  for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
	       if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
		    break;
	       } else if (token.Is(",")) {
		    continue;
	       } else if (is_register(token_table, token)) {
		    NIMONIC_INFO nim_info;
		    set_nimonic_with_register(token.AsString(), &nim_info);
		    const uint16_t nim = nim_info.prefix;
		    if (nim > 0x6600) {
			 const uint8_t first_byte  = nim & 0xff;
			 const uint8_t second_byte = (nim >> 8);
			 binout_container.push_back(second_byte);
			 binout_container.push_back(first_byte);
		    } else {
			 const uint8_t first_byte  = nim & 0xff;
			 const uint8_t second_byte = (nim >> 8);
			 binout_container.push_back(first_byte);
		    }
		    std::cout << "MOV REGISTER: " << token.AsString();

		    if (tokenizer.LookAhead(1).Is(",") && !tokenizer.LookAhead(2).IsEmpty()) {
			 // コンマを飛ばして次へ
			 token = tokenizer.Next();
			 token = tokenizer.Next();
			 std::cout << " <= " << token.AsString() << std::endl;

			 if (nim_info.imm == imm8) {
			      binout_container.push_back(token.AsLong());
			 } else if (nim_info.imm == imm16) {
			      set_word_into_binout(token.AsLong(), binout_container, false);
			 } else if (nim_info.imm == imm32) {
			      set_dword_into_binout(token.AsLong(), binout_container, false);
			 } else {
			      std::cerr << "NASK : MOV imm could not set correctly " << std::endl;
			      return 17;
			 }
		    }

	       } else {
		    binout_container.push_back(token.AsLong());
	       }
	  }
     }

     // 簡単なDB命令の実装
     int process_token_DB(TParaTokenizer& tokenizer, std::vector<uint8_t>& binout_container) {
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
     int process_token_DW(TParaTokenizer& tokenizer, std::vector<uint8_t>& binout_container) {
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
		    // DWを解釈
		    set_word_into_binout(token.AsLong(), binout_container);
	       }
	  }
     }

     // 簡単なDD命令の実装
     int process_token_DD(TParaTokenizer& tokenizer, std::vector<uint8_t>& binout_container) {
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
     }

     // 簡単なRESB命令の実装
     int process_token_RESB(TParaTokenizer& tokenizer, std::vector<uint8_t>& binout_container) {
	  for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
	       if (is_comment_line(token_table, token) || is_line_terminated(token_table, token)) {
		    break;
	       } else if (token.Is(",")) {
		    continue;
	       } else {
		    if (tokenizer.LookAhead(1).AsString() == "-" && tokenizer.LookAhead(2).AsString() == "$") {
			 // ハイフンを処理する、どこまで埋めるか取得する
			 for (ulong l = binout_container.size(); l < token.AsLong(); l++) {
			      binout_container.push_back(0x00);
			 }
			 // ハイフン処理は確定なのでtokenを進めておく
			 token = tokenizer.Next();
			 token = tokenizer.Next();
		    } else {
			 for (ulong l = 0; l < token.AsLong(); l++) {
			      binout_container.push_back(0x00);
			 }
		    }
	       }
	  }
     }
}
