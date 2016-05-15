#include <fstream>
#include <sstream>
#include "ParaTokenizer.hh"
#include "ParaOperator.hh"
#include "ParaExpression.hh"
#include "ParaSymbolTable.hh"
#include "ParaMathLibrary.hh"

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

     // uint16_tで数値を読み取った後、uint8_t型にデータを分けて
     // リトルエンディアンで格納する
     void set_word_into_binout(const uint16_t& word, std::vector<uint8_t>& binout_container) {

	  if (word == 0x0000) {
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

     // uint32_tで数値を読み取った後、uint8_t型にデータを分けて
     // リトルエンディアンで格納する
     void set_dword_into_binout(const uint32_t& dword, std::vector<uint8_t>& binout_container) {

	  if (dword == 0x00000000) {
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
