#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <getopt.h>
#include "ParaTokenizer.hh"
#include "ParaOperator.hh"
#include "ParaExpression.hh"
#include "ParaSymbolTable.hh"
#include "ParaMathLibrary.hh"
#include "nask_utility.hpp"
#include "bracket_utility.hpp"
#include "string_util.hpp"
#include <ctime>

using namespace std::placeholders;

constexpr unsigned long KILOBYTE = 1024;
constexpr unsigned long MEGABYTE = 1024 * 1024;
constexpr unsigned long GIGABYTE = 1024 * 1024 * 1024;

int process_each_assembly_line(char** argv,
			       std::ifstream& nas_file,
			       std::vector<uint8_t>& binout_container,
			       size_t start_line = 0) {

     /* 以下，入力の読み込みと解析，評価のループ */
     long line_number = 1;
     std::string input;

     // spdlog
     auto logger = spdlog::basic_logger_mt("opennask", "debug.log");



     static nask_utility::Instructions inst;
     static std::string current_symbol = "";
     using InstAlias = nask_utility::Instructions;

     static meta::funcs_type funcs {
	  std::make_pair("\["	  , std::bind(&InstAlias::process_token_BRACKET , inst, _1, _2)),
	  std::make_pair("ADD"	  , std::bind(&InstAlias::process_token_ADD	, inst, _1, _2)),
	  std::make_pair("ALIGNB" , std::bind(&InstAlias::process_token_ALIGNB  , inst, _1, _2)),
	  std::make_pair("AND"	  , std::bind(&InstAlias::process_token_AND	, inst, _1, _2)),
	  std::make_pair("CALL"	  , std::bind(&InstAlias::process_token_CALL	, inst, _1, _2)),
	  std::make_pair("CLI"	  , std::bind(&InstAlias::process_token_CLI	, inst, _1, _2)),
	  std::make_pair("CMP"	  , std::bind(&InstAlias::process_token_CMP	, inst, _1, _2)),
	  std::make_pair("DB"	  , std::bind(&InstAlias::process_token_DB	, inst, _1, _2)),
	  std::make_pair("DD"	  , std::bind(&InstAlias::process_token_DD	, inst, _1, _2)),
	  std::make_pair("DW"	  , std::bind(&InstAlias::process_token_DW	, inst, _1, _2)),
	  std::make_pair("GLOBAL" , std::bind(&InstAlias::process_token_GLOBAL  , inst, _1, _2)),
	  std::make_pair("HLT"	  , std::bind(&InstAlias::process_token_HLT	, inst, _1, _2)),
	  std::make_pair("IMUL"	  , std::bind(&InstAlias::process_token_IMUL	, inst, _1, _2)),
	  std::make_pair("IN"	  , std::bind(&InstAlias::process_token_IN	, inst, _1, _2)),
	  std::make_pair("INT"	  , std::bind(&InstAlias::process_token_INT	, inst, _1, _2)),
	  std::make_pair("JAE"	  , std::bind(&InstAlias::process_token_JAE	, inst, _1, _2)),
	  std::make_pair("JBE"	  , std::bind(&InstAlias::process_token_JBE	, inst, _1, _2)),
	  std::make_pair("JB"	  , std::bind(&InstAlias::process_token_JB	, inst, _1, _2)),
	  std::make_pair("JC"	  , std::bind(&InstAlias::process_token_JC	, inst, _1, _2)),
	  std::make_pair("JE"	  , std::bind(&InstAlias::process_token_JE	, inst, _1, _2)),
	  std::make_pair("JMP"	  , std::bind(&InstAlias::process_token_JMP	, inst, _1, _2)),
	  std::make_pair("JNC"	  , std::bind(&InstAlias::process_token_JNC	, inst, _1, _2)),
	  std::make_pair("JNZ"	  , std::bind(&InstAlias::process_token_JNZ	, inst, _1, _2)),
	  std::make_pair("JZ"	  , std::bind(&InstAlias::process_token_JZ	, inst, _1, _2)),
	  std::make_pair("LGDT"	  , std::bind(&InstAlias::process_token_LGDT	, inst, _1, _2)),
	  std::make_pair("MOV"	  , std::bind(&InstAlias::process_token_MOV	, inst, _1, _2)),
	  std::make_pair("NOP"	  , std::bind(&InstAlias::process_token_NOP	, inst, _1, _2)),
	  std::make_pair("OR"	  , std::bind(&InstAlias::process_token_OR	, inst, _1, _2)),
	  std::make_pair("ORG"	  , std::bind(&InstAlias::process_token_ORG	, inst, _1, _2)),
	  std::make_pair("OUT"	  , std::bind(&InstAlias::process_token_OUT	, inst, _1, _2)),
	  std::make_pair("POP"	  , std::bind(&InstAlias::process_token_POP	, inst, _1, _2)),
	  std::make_pair("POPF"	  , std::bind(&InstAlias::process_token_POPF 	, inst, _1, _2)),
	  std::make_pair("POPFD"  , std::bind(&InstAlias::process_token_POPF 	, inst, _1, _2)),
	  std::make_pair("PUSH"   , std::bind(&InstAlias::process_token_PUSH 	, inst, _1, _2)),
	  std::make_pair("PUSHF"  , std::bind(&InstAlias::process_token_PUSHF 	, inst, _1, _2)),
	  std::make_pair("PUSHFD" , std::bind(&InstAlias::process_token_PUSHF 	, inst, _1, _2)),
	  std::make_pair("RET"	  , std::bind(&InstAlias::process_token_RET	, inst, _1, _2)),
	  std::make_pair("RESB"	  , std::bind(&InstAlias::process_token_RESB	, inst, _1, _2)),
	  std::make_pair("SHR"	  , std::bind(&InstAlias::process_token_SHR	, inst, _1, _2)),
	  std::make_pair("STI"	  , std::bind(&InstAlias::process_token_STI	, inst, _1, _2)),
	  std::make_pair("SUB"	  , std::bind(&InstAlias::process_token_SUB	, inst, _1, _2))
     };

     if (start_line != 0) {
	  // 開始位置まで飛ばす
	  line_number = start_line;
	  for (size_t i = 0; i < start_line - 1; i++) {
	       std::getline(nas_file, input);
	  }
     }

     for (; std::getline(nas_file, input); line_number++) {

	  /* 行数チェック */
	  logger->info("{}: {}", line_number, input);

	  // シンボルテーブル用にシンボル情報を保持する
	  if (nask_utility::starts_with(input, "_")) {
	       using namespace std::regex_constants;
	       const std::regex symbol_reg("^(_[a-z0-9_]*):[^:]*$", extended | icase);
	       std::smatch match;

	       if (std::regex_search(input, match, symbol_reg) && match.size() > 1) {
		    if (current_symbol == "") {
			 inst.symbol_offsets["_"] = 0; // placeholder
			 inst.symbol_offsets[match[1].str()] = 0;
			 current_symbol = match[1].str();
			 logger->info("[detect] symbol: {}, offs size: {}", current_symbol, 0);
		    } else {
			 const size_t already_counted = inst.symbol_offsets["_"];
			 logger->info("[detect] symbol: {}, offs size: {}", current_symbol, already_counted);
			 inst.symbol_offsets[match[1].str()] = already_counted;
			 current_symbol = match[1].str();
		    }
	       }
	  }

	  // オペコードではなくラベルの可能性を探る(CRLF終わりの時が例外的なのでどうしたもんだか)
	  if (nask_utility::ends_with(input, ":") || nask_utility::ends_with(input, ":\r")) {
	       std::string label_dst = input.substr(0, input.find(":", 0));
	       logger->info("coming another label: {} bin[{}]",
			    label_dst, std::to_string(binout_container.size()));

               // label: (label_dstと呼ぶ)
               // 1) label_dstの位置を記録する → label_dst_stack
               // 2) 同名のlabel_srcが保存されていれば、オフセット値を計算して終了
	       inst.store_label_dst(label_dst, binout_container);
	       inst.update_label_dst_offset(label_dst, binout_container);
	       continue;
	  }

	  // 入力行にマクロが含まれていれば書き換える
	  for (const std::string pre_process_word : PRE_PROCESS_WORDS) {
	       std::size_t found = input.find(pre_process_word);
	       if (found != std::string::npos && pre_process_word == "EQU") {
		    logger->info("coming label EQU");
		    std::istringstream input_stream(input.c_str());
		    TParaTokenizer tokenizer(input_stream, &inst.token_table);
		    inst.process_token_EQU(tokenizer, binout_container);
		    continue;
	       }
	  }

          // 入力行に四則計算が含まれる場合、それを書き換える
	  std::string tmp = input;

	  if (nask_utility::is_contains_math_op(input)) {
	       // EQUで定義されている文字列があれば変換しておく
	       tmp = inst.try_replace_equ(input);
	       input = nask_utility::expr_math_op(tmp);
	       logger->info("{}: {} // *** math op replaced ***", line_number, input);
	  } else {
	       //
	       // 上で処理しきれなかったものを何とかする
	       //
	       tmp = inst.try_replace_equ(input);
	       if (nask_utility::is_contains_math_op(tmp)) {
		    input = nask_utility::expr_math_op(tmp);
		    logger->info("{}: {} // *** EQU & math op are replaced ***", line_number, input);
	       }
	  }

	  /* 入力行を istream にしてトークナイザを生成 */
	  std::istringstream input_stream(input.c_str());
	  TParaTokenizer tokenizer(input_stream, &inst.token_table);
	  TParaToken token;

	  if (nask_utility::starts_with(input, "\[")) {
	       logger->info("eval bracket");
	       try {
		    int r = inst.process_token_BRACKET(tokenizer, binout_container);
		    if (r != 0) {
			 // エラーがあった行を表示
			 logger->info(input);
			 return r;
		    }
	       } catch (TScriptException te) {
		    std::cerr << te << std::endl;
	       }
	       logger->info("eval bracket");
	       continue;
	  }

	  while (! (token = tokenizer.Next()).IsEmpty()) {
	       if (nask_utility::is_comment_line(inst.token_table, token) ||
		   nask_utility::is_line_terminated(inst.token_table, token)) {
		    break;
	       } else if (token.Is(",")) {
		    continue;
	       } else {
		    auto itr = std::find_if(std::begin(instruction),
					    std::end(instruction),
					    [&token] (const INST_TABLE& inst) -> bool {
						 return nask_utility::equals_ignore_case(
						      inst.opecode, token.AsString()
						      );
					    });

		    if( itr != instruction.end() ) {
			 //
			 // オペコードを処理する関数を探してきてそのまま実行
			 // （小文字でもちゃんと対象の処理を探せる）
			 //
			 meta::funcs_type::iterator it = funcs.find(token.AsString());
			 if (it != funcs.end()) {
			      logger->info("eval {}", token.AsString());

			      try {
				   const size_t before_process_size = binout_container.size();
				   const int r = it->second(tokenizer, binout_container);
				   const size_t offs = binout_container.size() - before_process_size;
				   if (current_symbol != "") {
					const size_t s = inst.symbol_offsets["_"];
					inst.symbol_offsets["_"] = s + offs;
					logger->info("symbol: {}, offs size: {}", current_symbol, s + offs);
				   }

				   if (r != 0) {
					// エラーがあった行を表示
					logger->info(input);
					return r;
				   }
			      } catch (TScriptException te) {
				   std::cerr << te << std::endl;
			      }
			      logger->info("eval {} end", token.AsString());
			 } else {
			      // オペコードが見つかったけどよくわからなかった場合
			      logger->info("eval Unknown {} end", token.AsString());
			 }
		    }
	       }
	  }
     }

     // sectionテーブルを追加する
     if (inst.exists_section_table) {
	  nask_utility::process_section_table(inst, binout_container);
     }

     logger->info("final size: bin[{}]", binout_container.size());
     return 0;
}


//
// FIXME: Currently, it's not working...
//
void nask_fdput(const char* img, std::vector<uint8_t>& binout_container) {

#ifdef BUILD_DOSFSTOOLS

#include "mkdosfs.h"

     // mkdosfs: see http://elm-chan.org/docs/fat.html
     //sector_size         = 512;  // BPB_BytsPerSec
     //sectors_per_cluster = 128;  // BPB_SecPerClus
#ifdef _WIN32
     static char dev_buf[] = "\\\\.\\X:";
#else
     struct stat statbuf;
#endif
     int i = 0, pos, ch;
     int create = 0;
     unsigned long long cblocks;

     time(&create_time);
     volume_id = (long)create_time;	/* Default volume ID = creation time */
     check_atari();

     // C : Create a new file
     create = TRUE;
     // F : Choose FAT size
     size_fat = 12;
     size_fat_by_user = 1;

     // s : Sectors per cluster
     sectors_per_cluster = 1;

     // S : Sector size
     sector_size = 512;
     sector_size_set = 1;
     blocks = 2880;

     off_t offset = blocks * BLOCK_SIZE - 1;
     char null = 0;
     /* create the file */
     dev = open( img, O_RDWR|O_CREAT|O_TRUNC, 0666 );
     if (dev < 0)
	  die("unable to create %s");
     /* seek to the intended end-1, and write one byte. this creates a
      * sparse-as-possible file of appropriate size. */
     if (llseek( dev, offset, SEEK_SET ) != offset)
	  die( "seek failed" );
     if (write( dev, &null, 1 ) < 0)
	  die( "write failed" );
     if (llseek( dev, 0, SEEK_SET ) != 0)
	  die( "seek failed" );

#ifdef _WIN32
     if (!is_device)
	  check = 0;
     establish_params();
#else
     if (fstat (dev, &statbuf) < 0)
	  die ("unable to stat %s");
     if (!S_ISBLK (statbuf.st_mode)) {
	  statbuf.st_rdev = 0;
	  check = 0;
     }
     else
	  /*
	   * Ignore any 'full' fixed disk devices, if -I is not given.
	   * On a MO-disk one doesn't need partitions.  The filesytem can go
	   * directly to the whole disk.  Under other OSes this is known as
	   * the 'superfloppy' format.  As I don't know how to find out if
	   * this is a MO disk I introduce a -I (ignore) switch.  -Joey
	   */
	  if (!ignore_full_disk && (
		   (statbuf.st_rdev & 0xff3f) == 0x0300 || /* hda, hdb */
		   (statbuf.st_rdev & 0xff0f) == 0x0800 || /* sd */
		   (statbuf.st_rdev & 0xff3f) == 0x0d00 || /* xd */
		   (statbuf.st_rdev & 0xff3f) == 0x1600 )  /* hdc, hdd */
	       )
	       die ("Will not try to make filesystem on full-disk device '%s' (use -I if wanted)");

     establish_params (statbuf.st_rdev,statbuf.st_size);
     /* Establish the media parameters */
#endif

     setup_tables (sectors_per_cluster);	/* Establish the file system tables */
     write_tables ();		/* Write the file system tables away! */

#ifdef _WIN32
     if (is_device) {
	  if (fsctl(dev, FSCTL_DISMOUNT_VOLUME) == -1)
	       die("unable to dismount %s");
	  if (fsctl(dev, FSCTL_UNLOCK_VOLUME) == -1)
	       die("unable to unlock %s");
     }
#endif

#endif

     return;
}

int main(int argc, char** argv) {

     int opt, i, option_index;
     bool with_fat12 = false;

     struct option long_options[] = {
	  {"with-fat12", no_argument,        NULL, 'f'},
	  {"help",       no_argument,        NULL, 'h'},
	  {0, 0, 0, 0}// 配列の最後はすべて0で埋める
     };

     while((opt = getopt_long(argc, argv, "mes:", long_options, &option_index)) != -1){
	  switch(opt){
	  case 'f':
	       with_fat12 = true;
	       break;
	  case 'h':
	       printf("usage:  [with-fat12 | help] source [object/binary] [list]\n");
	       return 0;
	       // 解析できないオプションが見つかった場合は「?」を返す
	       // オプション引数が不足している場合も「?」を返す
	  case '?':
	       printf("unknown or required argument option -%c\n", optopt);
	       printf("usage:  [with-fat12 | help] source [object/binary] [list]\n");
	       return 1;   // exit(EXIT_FAILURE);と同等 http://okwave.jp/qa/q794746.html
	  }
     }

     if (argc - optind < 2 || argc - optind > 4) {
	  std::cerr << "usage:  [--with-fat12 | --help] source [object/binary] [list]" << std::endl;
	  return 16;
     }
     const char* assembly_src = argv[optind];
     const char* assembly_dst = argv[optind + 1];

     const size_t len = nask_utility::filesize(assembly_src);
     if (len == -1) {
	  std::cerr << "NASK : can't open " << assembly_src << std::endl;
	  return 17;
     }

     /* 入力するアセンブラ情報 */
     std::ifstream nas_file;
     nas_file.open(assembly_src);
     if ( nas_file.bad() || nas_file.fail() ) {
	  std::cerr << "NASK : can't read " << assembly_src << std::endl;
	  return 17;
     }

     /* 出力するバイナリ情報 */
     std::vector<uint8_t> binout_container;
     std::ofstream binout(assembly_dst, std::ios::trunc | std::ios::binary);
     if ( binout.bad() || binout.fail() ) {
	  std::cerr << "NASK : can't open " << assembly_dst << std::endl;
	  return 17;
     }

     // 入力の読み込みと解析，評価のループ
     process_each_assembly_line(argv, nas_file, binout_container);

     if (with_fat12) {
	  // output binary in FAT12 disk image file
	  binout.flush();
	  binout.close();
	  nask_fdput(assembly_dst, binout_container);
     } else {
	  // output binary
	  binout.write(reinterpret_cast<char*>(binout_container.data()), binout_container.size());
	  binout.close();
     }

     return 0;
}
