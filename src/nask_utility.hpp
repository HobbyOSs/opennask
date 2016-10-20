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
#include "spdlog/spdlog.h"

// 80x86 Opcodes
// https://courses.engr.illinois.edu/ece390/resources/opcodes.html
const std::array<std::string, 24> REGISTERS {
     "AL", "BL", "CL", "DL", "EAX", "EBX", "ECX", "EDX", "AX", "BX", "CX", "DX",
     "AH", "BH", "CH", "DH", "ESP", "EDI", "EBP", "ESI", "SP", "DI", "BP", "SI"
};

const std::array<std::string, 6> SEGMENT_REGISTERS {
     "CS", // コード
     "DS", // データ
     "ES", // エクストラ
     "SS", // スタック
     "FS",
     "GS"
};

//
// The registers FS and GS are segment registers. They have no processor-defined purpose,
// but instead are given purpose by the OS's running them.
// In Windows 64-bit the GS register is used to point to operating system defined structures.
// FS and GS are commonly used by OS kernels to access thread-specific memory. In windows,
// the GS register is used to manage thread-specific memory.
// The linux kernel uses GS to access cpu-specific memory.
//
// https://stackoverflow.com/questions/10810203/what-is-the-fs-gs-register-intended-for/10810287#10810287

const std::array<std::string, 4> CONTROL_REGISTERS {
     "CR0", "CR2", "CR3", "CR4"
};

const std::array<std::string, 1> PRE_PROCESS_WORDS {
     "EQU"
};

const std::array<std::string, 4> PRE_PROCESS_OPERATORS {
     "*", "/", "+", "-"
};

const std::array<std::string, 10> DATA_TYPES {
     "BYTE", "WORD", "DWORD", "FWORD", "QWORD", "TBYTE", "OWORD", "REAL4", "REAL8", "REAL10"
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
struct LABEL_DST_ELEMENT {
  std::string label; // ex) entry:
  int src_index;  // JMPのオペコードが始まる場所
  int dst_index;  // JMPの飛び先のラベルが始まる場所
  int rel_index;  // rel_offsetを格納する場所
  int rel_offset() {
       // offset = rel - dst
       spdlog::get("opennask")->info("{} - {} - 1", std::to_string(src_index), std::to_string(rel_index));
       return src_index - rel_index - 1;
  };
};

struct LABEL_SRC_ELEMENT {
  std::string label;   // ex) entry
  OPERAND_KINDS operand;
  bool abs = false;
  int src_index;  // JMPのオペコードが始まる場所
  int dst_index;  // JMPの飛び先のラベルが始まる場所
  int rel_index;  // rel_offsetを格納する場所
  int rel_offset() {
       // offset = rel - dst
       spdlog::get("opennask")->info("{} - {} - 1", std::to_string(dst_index), std::to_string(rel_index));
       return dst_index - rel_index - 1;
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

     // 処理の中でlabel情報の収集をする
     typedef std::vector<LABEL_DST_ELEMENT> LABEL_DST_STACK;
     typedef std::vector<LABEL_SRC_ELEMENT> LABEL_SRC_STACK;
     // 出力先
     typedef std::vector<uint8_t> VECTOR_BINOUT;

     // Javaにはありがちな基本的文字列処理
     std::ifstream::pos_type filesize(const char* filename);
     std::vector<std::string> split(const std::string &str, char delim);
     std::string replace(std::string& str, const std::string& from, const std::string& to);
     bool contains(const std::string& src, const std::string& query);
     bool ends_with(std::string const &full_string, std::string const &ending);

     // アセンブラ処理で使う判定系処理
     bool is_hex_notation(const std::string& s);
     bool is_integer(const std::string& s);
     bool is_legitimate_numeric(const std::string& s);
     bool is_between_bytesize(const long l);
     bool is_imm8(const std::string& token);
     bool is_contains_math_op(const std::string& subject);
     std::string expr_math_op(const std::string& subject);
     size_t get_imm_size(const std::string& hex_string);

     size_t get_labelpos(std::ifstream& stream, std::string token);
     bool is_comment_line(TParaCxxTokenTable& token_table, TParaToken& token);
     bool is_line_terminated(TParaCxxTokenTable& token_table, TParaToken& token);
     bool is_common_register(TParaCxxTokenTable& token_table, const TParaToken& token);
     bool is_segment_register(TParaCxxTokenTable& token_table, const TParaToken& token);
     bool is_control_register(TParaCxxTokenTable& token_table, const TParaToken& token);
     bool is_register(TParaCxxTokenTable& token_table, const TParaToken& token);
     template<size_t N> bool is_registers_with_args(const TParaToken& token, const std::array<std::string, N>& regs);

     bool is_datatype(TParaCxxTokenTable& token_table, const TParaToken& token);

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
			       VECTOR_BINOUT& binout_container,
			       bool zero_as_byte = true);

     // uint32_tで数値を読み取った後、uint8_t型にデータを分けて、リトルエンディアンで格納する
     // nask的にはDDは0x00を普通に詰めるらしい（仕様ブレブレすぎだろ…）
     void set_dword_into_binout(const uint32_t& dword,
				VECTOR_BINOUT& binout_container,
				bool zero_as_byte = false);

     // アセンブラ命令処理
     class Instructions {
     public:
	  static TParaCxxTokenTable token_table;
	  static LABEL_DST_STACK label_dst_stack;
	  static LABEL_SRC_STACK label_src_stack;
	  static std::map<std::string, std::string> equ_map;
	  static std::string data_type;
	  static uint32_t dollar_position; // $
	  int OPENNASK_MODES = ID_32BIT_MODE;

	  int process_token_ADD (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_AND (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_CALL(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_CLI (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_CMP (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_DB  (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_DD  (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_DW  (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_EQU (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_HLT (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_IMUL(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_IN  (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_INT (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_JAE (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_JBE (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_JB  (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_JC  (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_JE  (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_JMP (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_JNC (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_LGDT(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_MOV (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_NOP (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_OR  (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_ORG (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_OUT (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_RESB(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_SUB (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);

	  // relX に関する関数
	  void store_label_dst(std::string label_dst, VECTOR_BINOUT& binout_container);
	  void update_label_dst_offset(std::string label_dst, VECTOR_BINOUT& binout_container);
	  bool dst_is_stored(std::string label_dst, VECTOR_BINOUT& binout_container);

	  void store_label_src(std::string label_src, VECTOR_BINOUT& binout_container, bool abs = false);
	  bool update_label_src_offset(std::string label_src, VECTOR_BINOUT& binout_container, uint8_t nim);
	  // EQUで保存されているラベルの実体を取り出すか、そのまま返す
	  std::string get_equ_label_or_asis(std::string key);
     };
}

namespace meta {
     // from: http://faithandbrave.hateblo.jp/entry/20071026/1193404885
     typedef std::function<int(TParaTokenizer &, nask_utility::VECTOR_BINOUT &)> nim_callback;
     typedef std::map<std::string, nim_callback> funcs_type;
};

#endif /* NASK_UTILITY_HPP_ */
