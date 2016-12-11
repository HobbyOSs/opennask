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
  long src_index;  // JMPのオペコードが始まる場所
  long dst_index;  // JMPの飛び先のラベルが始まる場所
  long rel_index;  // rel_offsetを格納する場所
  long rel_offset() {
       // offset = rel - dst
       spdlog::get("opennask")->info("dst_offs: {} - {} - 1", std::to_string(src_index), std::to_string(rel_index));
       return src_index - rel_index - 1;
  };
};

struct LABEL_SRC_ELEMENT {
  std::string label;   // ex) entry
  OPERAND_KINDS operand;
  bool abs = false;
  long src_index;  // JMPのオペコードが始まる場所
  long dst_index;  // JMPの飛び先のラベルが始まる場所
  long rel_index;  // rel_offsetを格納する場所
  size_t offset_size; // オフセットの格納サイズを指定する
  long rel_offset() {
       // offset = rel - dst
       spdlog::get("opennask")->info("src_offs: {} - {} - 1", std::to_string(dst_index), std::to_string(rel_index));
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
			       bool zero_as_byte = true,
			       size_t start_index = 0);

     // uint32_tで数値を読み取った後、uint8_t型にデータを分けて、リトルエンディアンで格納する
     // nask的にはDDは0x00を普通に詰めるらしい（NASMの仕様かな？）
     //
     // @param dword            格納するDWORDサイズのバイナリ
     // @param binout_container 出力先コンテナ
     // @param zero_as_byte     0x00をバイトサイズで格納する
     // @param start_index      格納するコンテナの開始index
     void set_dword_into_binout(const uint32_t& dword,
				VECTOR_BINOUT& binout_container,
				bool zero_as_byte = false,
				size_t start_index = 0);

     // "0x0A0A"のような文字列をBYTEサイズで順次コンテナに詰め込む
     // @param dword            格納するDWORDサイズのバイナリ
     // @param binout_container 出力先コンテナ
     // @param reverse          true: 逆順でバイナリを登録, false: そのまま登録
     void set_hexstring_into_binout(const std::string& in,
				    VECTOR_BINOUT& binout_container, bool reverse = true);

     // アセンブラ命令処理
     class Instructions {
     public:
	  Instructions();

	  static TParaCxxTokenTable token_table;
	  static LABEL_DST_STACK label_dst_stack;
	  static LABEL_SRC_STACK label_src_stack;
	  static std::map<std::string, std::string> equ_map;
	  static std::vector<std::string> symbol_list;
	  static std::map<std::string, size_t> symbol_offsets;
	  static std::string data_type;
	  static std::map<uint32_t, std::string> support_cpus;

	  // Current Position '$', INSTRSET
	  static uint32_t dollar_position, support;
	  static int OPENNASK_MODES;

	  // section table defs
	  bool exists_section_table = false;
	  bool exists_file_auxiliary = false;
	  std::string file_auxiliary_name = "";
	  NAS_PIMAGE_SYMBOL file_auxiliary;

	  int process_token_BRACKET(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_ADD    (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_ALIGNB (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_AND    (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_CALL   (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_CLI    (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_CMP    (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_DB     (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_DD     (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_DW     (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_EQU    (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_GLOBAL (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_HLT    (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_IMUL   (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_IN     (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_INT    (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_JAE    (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_JBE    (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_JB     (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_JC     (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_JE     (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_JMP    (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_JNC    (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_JNZ    (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_JZ     (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_LGDT   (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_LIDT   (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_MOV    (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_NOP    (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_OR     (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_ORG    (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_OUT    (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_POP    (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_POPA   (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_POPF   (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_PUSH   (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_PUSHA  (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_PUSHF  (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_RET    (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_RESB   (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_SHR    (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_STI    (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);
	  int process_token_SUB    (TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container);

	  // register-size prefix
	  void store_register_size_prefix(const std::string& src_reg, VECTOR_BINOUT& binout_container);

	  // relX に関する関数
	  void store_label_dst(std::string label_dst, VECTOR_BINOUT& binout_container);
	  void update_label_dst_offset(std::string label_dst, VECTOR_BINOUT& binout_container);
	  bool dst_is_stored(std::string label_dst);

	  void store_label_src(std::string label_src,VECTOR_BINOUT& binout_container, bool abs = false, size_t offset_size = imm8);
	  const long get_label_src_offset(std::string label_src); // DB, DW, DD用
	  bool update_label_src_offset(std::string label_src, VECTOR_BINOUT& binout_container, uint8_t nim);
	  bool update_label_src_offset(std::string label_src, VECTOR_BINOUT& binout_container);
	  // EQUで保存されているラベルの実体を取り出すか、そのまま返す
	  std::string get_equ_label_or_asis(std::string key);
	  std::vector<std::string> get_equ_keyset();
	  std::string try_replace_equ(const std::string& line);

	  // FIXME: MOV命令の際のユーティリティコード、後で無くす
	  void set_nimonic_with_register(const std::string& reg,
					 NIMONIC_INFO* nim_info,
					 TParaTokenizer& tokenizer);
     };
}

namespace meta {

     // from: http://stackoverflow.com/a/19102250/2565527
     struct comp {
	  bool operator() (const std::string& lhs, const std::string& rhs) const {
	       return strcasecmp(lhs.c_str(), rhs.c_str()) < 0;
	  }
     };

     // from: http://faithandbrave.hateblo.jp/entry/20071026/1193404885
     typedef std::function<int(TParaTokenizer &, nask_utility::VECTOR_BINOUT &)> nim_callback;
     typedef std::map<std::string, nim_callback, comp> funcs_type;

     // from: http://stackoverflow.com/a/14807477/2565527
     // like Ruby's hash#invert
     template<typename A, typename B>
     std::pair<B,A> flip_pair(const std::pair<A,B> &p) {
	  return std::pair<B,A>(p.second, p.first);
     }

     template<typename A, typename B, typename Comp> // discard Comp
     std::map<B,A> flip_map(const std::map<A,B,Comp> &src) {
	  std::map<B,A> dst;
	  std::transform(src.begin(), src.end(), inserter(dst, dst.begin()), flip_pair<A,B>);
	  return dst;
     }
};

#endif /* NASK_UTILITY_HPP_ */
