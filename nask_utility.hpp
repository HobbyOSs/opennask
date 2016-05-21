#ifndef NASK_UTILITY_HPP_
#define NASK_UTILITY_HPP_

#include <fstream>
#include <sstream>
#include <algorithm>
#include "ParaTokenizer.hh"
#include "ParaOperator.hh"
#include "ParaExpression.hh"
#include "ParaSymbolTable.hh"
#include "ParaMathLibrary.hh"
#include "nask_defs.hpp"
#include <functional>

// 80x86 Opcodes
// https://courses.engr.illinois.edu/ece390/resources/opcodes.html
const std::array<std::string, 24> REGISTERS {
     "AL", "BL", "CL", "DL", "EAX", "EBX", "ECX", "EDX", "AX", "BX", "CX", "DX",
     "AH", "BH", "CH", "DH", "ESP", "EDI", "EBP", "ESI", "SP", "DI", "BP", "SI"
};

const std::array<std::string, 4> SEGMENT_REGISTERS {
     "CS", // コード
     "DS", // データ
     "ES", // エクストラ
     "SS"  // スタック
};

// 0xEB cb JMP rel8	次の命令との相対オフセットだけ相対ショートジャンプする
// 0xE9 cw JMP rel16	次の命令との相対オフセットだけ相対ニアジャンプする
// 0xE9 cd JMP rel32	次の命令との相対オフセットだけ相対ニアジャンプする
// 0xFF /4 JMP r/m16	r/m16で指定されるアドレスに絶対間接ニアジャンプする
// 0xFF /4 JMP r/m32	r/m32で指定されるアドレスに絶対間接ニアジャンプする
// 0xEA cd JMP ptr16:16	オペランドで指定されるアドレスに絶対ファージャンプする
// 0xEA cp JMP ptr16:32	オペランドで指定されるアドレスに絶対ファージャンプする
// 0xFF /5 JMP m16:16	m16:16で指定されるアドレスに絶対間接ファージャンプする
// 0xFF /5 JMP m16:32	m16:32で指定されるアドレスに絶対間接ファージャンプする
struct JMP_STACK_ELEMENT {
  std::string label; // ex) entry:
  size_t src_index;  // JMPのオペコードが始まる場所
  size_t dst_index;  // JMPの飛び先のラベルが始まる場所
  size_t rel_index;  // rel_offsetを格納する場所
  size_t rel_offset() {
       // offset = destination - source + sizeof(opcode)
       // sizeof(opcode)はとりあえず2byteにしとく
       return dst_index - src_index + 0x02;
  };
};

// MOV DEST, SRC
//     動作：DEST←SRC
//     DEST：レジスタ、メモリー
//     SRC ：レジスタ、メモリー、即値（ただしメモリー、メモリーの組み合わせは除く）
struct NIMONIC_INFO {
     std::string reg; // レジスタ名
     size_t imm;      // オペコード内におけるSRCのバイトサイズ
     uint16_t prefix; // オペコードを機械語で表す際の接頭辞
};

namespace nask_utility {

     // 処理の中でJMP情報の収集をする
     typedef std::vector<JMP_STACK_ELEMENT> JMP_STACK;

     std::ifstream::pos_type filesize(const char* filename);
     std::vector<std::string> split(const std::string &str, char delim);
     bool ends_with(std::string const &full_string, std::string const &ending);
     size_t get_labelpos(std::ifstream& stream, std::string token);
     bool is_comment_line(TParaCxxTokenTable& token_table, TParaToken& token);
     bool is_line_terminated(TParaCxxTokenTable& token_table, TParaToken& token);
     bool is_register(TParaCxxTokenTable& token_table, TParaToken& token);
     bool is_segment_register(TParaCxxTokenTable& token_table, TParaToken& token);

     template <class T> void plus_number_from_code(T& num, char c);
     uint8_t get_plus_register_code(uint8_t byte, char c);
     uint16_t get_plus_register_code(uint16_t word, char c);
     uint32_t get_plus_register_code(uint32_t dword, char c);

     void set_nimonic_with_register(const std::string& reg,
				    NIMONIC_INFO* nim_info,
				    TParaTokenizer& tokenizer);

     // @detail uint16_tで数値を読み取った後、uint8_t型にデータを分けて、リトルエンディアンで格納する
     // nask的には0x00をバイトサイズで格納する傾向があるので、そうじゃない場合はフラグを設定する
     //
     // @param word             格納するWORDサイズのバイナリ
     // @param binout_container 出力先コンテナ
     // @param zero_as_byte     0x00をバイトサイズで格納する
     //
     void set_word_into_binout(const uint16_t& word,
			       std::vector<uint8_t>& binout_container,
			       bool zero_as_byte = true);

     // uint32_tで数値を読み取った後、uint8_t型にデータを分けて、リトルエンディアンで格納する
     // nask的にはDDは0x00を普通に詰めるらしい（仕様ブレブレすぎだろ…）
     void set_dword_into_binout(const uint32_t& dword,
				std::vector<uint8_t>& binout_container,
				bool zero_as_byte = false);

     // アセンブラ命令処理
     class Instructions {
     public:
	  static TParaCxxTokenTable token_table;
	  static JMP_STACK stack;
	  static size_t dollar_position; // $
	  int OPENNASK_MODES = ID_32BIT_MODE;

	  int process_token_MOV (TParaTokenizer& tokenizer, std::vector<uint8_t>& binout_container);
	  int process_token_JMP (TParaTokenizer& tokenizer, std::vector<uint8_t>& binout_container);
	  int process_token_DB  (TParaTokenizer& tokenizer, std::vector<uint8_t>& binout_container);
	  int process_token_DW  (TParaTokenizer& tokenizer, std::vector<uint8_t>& binout_container);
	  int process_token_DD  (TParaTokenizer& tokenizer, std::vector<uint8_t>& binout_container);
	  int process_token_RESB(TParaTokenizer& tokenizer, std::vector<uint8_t>& binout_container);

	  void set_jmp_stack(std::string store_label, std::vector<uint8_t>& binout_container);
	  void update_jmp_stack(std::string found_label, std::vector<uint8_t>& binout_container);
     };

     namespace ModRM {
	  const std::regex rm000("(AL|AX|EAX)");
	  const std::regex rm001("(CL|CX|ECX)");
	  const std::regex rm010("(DL|DX|EDX)");
	  const std::regex rm011("(BL|BX|EBX)");

	  enum mods {
	       REG_REG = 0, // mod=00: [レジスター+レジスター]
	       REG_DISP8,   // mod=01: [レジスター+disp8]
	       REG_DISP16,  // mod=10: [レジスター+disp16/32]
	       REG          // mod=11: レジスター
	  };

	  const std::map<enum mods, std::string> MOD_TO_STR {
	       { mods::REG_REG    , "00"},
	       { mods::REG_DISP8  , "01"},
	       { mods::REG_DISP16 , "10"},
	       { mods::REG	     , "11"}
	  };

	  // sss : Segment Register
	  //-----------------------
	  // 000 : ES
	  // 001 : CS
	  // 010 : SS
	  // 011 : DS
	  // 100 : FS (Only 386+)
	  // 101 : GS (Only 386+)
	  const std::map<std::string, std::string> SEGMENT_REGISTERS_SSS_MAP {
	       { "ES", "000"},
	       { "CS", "001"},
	       { "SS", "010"},
	       { "DS", "011"},
	       { "FS", "100"},
	       { "GS", "101"}
	  };

	  // @see: https://courses.engr.illinois.edu/ece390/resources/opcodes.html
	  // rrr : W=0 : W=1 : reg32
	  //------------------------
	  // 000 : AL  : AX  : EAX
	  // 001 : CL  : CX  : ECX
	  // 010 : DL  : DX  : EDX
	  // 011 : BL  : BX  : EBX
	  // 100 : AH  : SP  : ESP
	  // 101 : CH  : BP  : EBP
	  // 110 : DH  : SI  : ESI
	  // 111 : BH  : DI  : EDI
	  const std::map<std::string, std::tuple<std::string, std::string>> REGISTERS_RRR_MAP {
	       // reg:                [rrr,   W]
	       { "AL", std::make_tuple("000", "0") },
	       { "CL", std::make_tuple("001", "0") },
	       { "DL", std::make_tuple("010", "0") },
	       { "BL", std::make_tuple("011", "0") },
	       { "AH", std::make_tuple("100", "0") },
	       { "CH", std::make_tuple("101", "0") },
	       { "DH", std::make_tuple("110", "0") },
	       { "BH", std::make_tuple("111", "0") },
	       { "AX", std::make_tuple("000", "1") },
	       { "CX", std::make_tuple("001", "1") },
	       { "DX", std::make_tuple("010", "1") },
	       { "BX", std::make_tuple("011", "1") },
	       { "SP", std::make_tuple("100", "1") },
	       { "BP", std::make_tuple("101", "1") },
	       { "SI", std::make_tuple("110", "1") },
	       { "DI", std::make_tuple("111", "1") },
	  };

	  const std::string get_rm_from_reg(const std::string& src_reg);
	  uint8_t generate_modrm(enum mods m, const std::string& dst_reg, const std::string& src_reg);
     };
}

namespace meta {
     // from: http://faithandbrave.hateblo.jp/entry/20071026/1193404885
     typedef std::function<int(TParaTokenizer &, std::vector<uint8_t> &)> nim_callback;
     typedef std::map<std::string, nim_callback> funcs_type;

     //struct INST_SINGLETON {
     // 	  static meta::funcs_type funcs;
     // 	  static nask_utility::Instructions inst;
     //};

     //static void get_instance(TParaCxxTokenTable& token_table, INST_SINGLETON* singleton) {
     //};
};

#endif /* NASK_UTILITY_HPP_ */
