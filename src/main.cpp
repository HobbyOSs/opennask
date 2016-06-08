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
#include "clogger.hpp"
#include "fatlib.h"
#include "time.h"
#include <errno.h>
#include <err.h>
#include <utime.h>
#include <unistd.h>
#include <sys/types.h>

using namespace std::placeholders;

constexpr unsigned long BYTE     = 1;
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

     // デフォルトのトークンテーブル
     TParaCxxTokenTable token_table;
     token_table.AddCommentLimiter(";", "\n");
     token_table.AddCommentLimiter("#", "\n");

     static meta::funcs_type funcs;
     static nask_utility::Instructions inst;
     inst.token_table = token_table;

     // このへんマクロかtemplateを使いたい
     const auto fp_ADD  = std::bind(&nask_utility::Instructions::process_token_ADD  , inst, _1, _2);
     const auto fp_CMP  = std::bind(&nask_utility::Instructions::process_token_CMP  , inst, _1, _2);
     const auto fp_DB   = std::bind(&nask_utility::Instructions::process_token_DB   , inst, _1, _2);
     const auto fp_DD   = std::bind(&nask_utility::Instructions::process_token_DD   , inst, _1, _2);
     const auto fp_DW   = std::bind(&nask_utility::Instructions::process_token_DW   , inst, _1, _2);
     const auto fp_HLT  = std::bind(&nask_utility::Instructions::process_token_HLT  , inst, _1, _2);
     const auto fp_INT  = std::bind(&nask_utility::Instructions::process_token_INT  , inst, _1, _2);
     const auto fp_JAE  = std::bind(&nask_utility::Instructions::process_token_JAE  , inst, _1, _2);
     const auto fp_JBE  = std::bind(&nask_utility::Instructions::process_token_JBE  , inst, _1, _2);
     const auto fp_JC   = std::bind(&nask_utility::Instructions::process_token_JC   , inst, _1, _2);
     const auto fp_JE   = std::bind(&nask_utility::Instructions::process_token_JE   , inst, _1, _2);
     const auto fp_JMP  = std::bind(&nask_utility::Instructions::process_token_JMP  , inst, _1, _2);
     const auto fp_JNC  = std::bind(&nask_utility::Instructions::process_token_JNC  , inst, _1, _2);
     const auto fp_MOV  = std::bind(&nask_utility::Instructions::process_token_MOV  , inst, _1, _2);
     const auto fp_ORG  = std::bind(&nask_utility::Instructions::process_token_ORG  , inst, _1, _2);
     const auto fp_RESB = std::bind(&nask_utility::Instructions::process_token_RESB , inst, _1, _2);
     funcs.insert(std::make_pair("ADD" , fp_ADD));
     funcs.insert(std::make_pair("CMP" , fp_CMP));
     funcs.insert(std::make_pair("DB"  , fp_DB));
     funcs.insert(std::make_pair("DD"  , fp_DD));
     funcs.insert(std::make_pair("DW"  , fp_DW));
     funcs.insert(std::make_pair("HLT" , fp_HLT));
     funcs.insert(std::make_pair("INT" , fp_INT));
     funcs.insert(std::make_pair("JAE" , fp_JAE));
     funcs.insert(std::make_pair("JBE" , fp_JBE));
     funcs.insert(std::make_pair("JC"  , fp_JC));
     funcs.insert(std::make_pair("JE"  , fp_JE));
     funcs.insert(std::make_pair("JMP" , fp_JMP));
     funcs.insert(std::make_pair("JNC" , fp_JNC));
     funcs.insert(std::make_pair("MOV" , fp_MOV));
     funcs.insert(std::make_pair("ORG" , fp_ORG));
     funcs.insert(std::make_pair("RESB", fp_RESB));

     if (start_line != 0) {
	  // 開始位置まで飛ばす
	  line_number = start_line;
	  for (size_t i = 0; i < start_line - 1; i++) {
	       std::getline(nas_file, input);
	  }
     }

     for (; std::getline(nas_file, input); line_number++) {

	  /* 行数チェック */
	  std::cout.setf(std::ios::dec, std::ios::basefield);
	  std::cout << line_number << ": " << input << std::endl;
	  // オペコードではなくラベルの可能性を探る(CRLF終わりの時が例外的なのでどうしたもんだか)
	  if (nask_utility::ends_with(input, ":") || nask_utility::ends_with(input, ":\r")) {
	       std::string label_dst = input.substr(0, input.find(":", 0));
	       std::cout << "coming another label: "
			 << label_dst
			 << " bin["
			 << std::to_string(binout_container.size())
			 << "]"
			 << std::endl;

               // label: (label_dstと呼ぶ)
               // 1) label_dstの位置を記録する → label_dst_stack
               // 2) 同名のlabel_srcが保存されていれば、オフセット値を計算して終了
	       inst.store_label_dst(label_dst, binout_container);
	       inst.update_label_dst_offset(label_dst, binout_container);
	       continue;
	  }

	  /* 入力行を istream にしてトークナイザを生成 */
	  std::istringstream input_stream(input.c_str());
	  TParaTokenizer tokenizer(input_stream, &token_table);
	  TParaToken token;

	  while (! (token = tokenizer.Next()).IsEmpty()) {
	       if (nask_utility::is_comment_line(token_table, token) ||
		   nask_utility::is_line_terminated(token_table, token)) {
		    break;
	       } else if (token.Is(",")) {
		    continue;
	       } else {
		    auto itr = std::find_if(std::begin(instruction),
					    std::end(instruction),
					    [&token] (const INST_TABLE& inst) -> bool {
						 return inst.opecode == token.AsString();
					    });

		    if( itr != instruction.end() ) {
			 //
			 // オペコードを処理する関数を探してきてそのまま実行
			 //
			 meta::funcs_type::iterator it = funcs.find(token.AsString());
			 if (it != funcs.end()) {
			      std::cout << "eval " << token.AsString() << std::endl;
			      try {
				   int r = it->second(tokenizer, binout_container);
			      } catch (TScriptException te) {
				   std::cerr << te << std::endl;
			      }
			      std::cout << "eval " << token.AsString() <<" end" << std::endl;
			 } else {
			      // What
			 }
		    }
	       }
	  }
     }
     return 0;
}

#include "mkdosfs.h"

//
// FIXME: Currently, it's not working...
//
void nask_fdput(const char* img, std::vector<uint8_t>& binout_container) {

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

     // clogger
#ifndef __clang__
     std::ofstream file("debug.log", std::ios::out | std::ios::trunc);
     clogger logger(file.rdbuf());
     std::streambuf* oldrdbuf = std::cout.rdbuf(&logger);
#endif

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

     std::cout << std::endl;

     return 0;
}
