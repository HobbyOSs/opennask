#include <iostream>
#include <strstream>
#include <algorithm>
#include <fstream>
#include "ParaTokenizer.hh"
#include "ParaOperator.hh"
#include "ParaExpression.hh"
#include "ParaSymbolTable.hh"
#include "ParaMathLibrary.hh"
#include "nask_utility.hpp"

using namespace std::placeholders;

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

     const auto fp_MOV  = std::bind(&nask_utility::Instructions::process_token_MOV  , inst, _1, _2);
     const auto fp_JMP  = std::bind(&nask_utility::Instructions::process_token_JMP  , inst, _1, _2);
     const auto fp_DB   = std::bind(&nask_utility::Instructions::process_token_DB   , inst, _1, _2);
     const auto fp_DW   = std::bind(&nask_utility::Instructions::process_token_DW   , inst, _1, _2);
     const auto fp_DD   = std::bind(&nask_utility::Instructions::process_token_DD   , inst, _1, _2);
     const auto fp_RESB = std::bind(&nask_utility::Instructions::process_token_RESB , inst, _1, _2);
     funcs.insert(std::make_pair("MOV" , fp_MOV));
     funcs.insert(std::make_pair("JMP" , fp_JMP));
     funcs.insert(std::make_pair("DB"  , fp_DB));
     funcs.insert(std::make_pair("DW"  , fp_DW));
     funcs.insert(std::make_pair("DD"  , fp_DD));
     funcs.insert(std::make_pair("RESB", fp_RESB));

     if (start_line != 0) {
	  // 開始位置まで飛ばす
	  line_number = start_line;
	  for (size_t i = 0; i < start_line - 1; i++) {
	       std::getline(nas_file, input);
	  }
     }

     for (line_number; std::getline(nas_file, input); line_number++) {

	  /* 行数チェック */
	  std::cout << line_number << ": " << input << std::endl;

	  /* 入力行を istream にしてトークナイザを生成 */
	  std::istrstream input_stream(input.c_str());
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
			 }

		    } else {

			 // オペコードではなくラベルの可能性を探る
			 if (nask_utility::ends_with(token.AsString(), ":")) {
			      std::cout << "coming another label" << std::endl;
			      std::string label = token.AsString();
			      inst.update_jmp_stack(label.substr(0, label.size() - 1), binout_container);
			 }
		    }
	       }
	  }
     }
}

int main(int argc, char** argv) {

     if (argc < 2 || argc > 4) {
	  std::cerr << "usage : >opennask source [object/binary] [list]" << std::endl;
	  return 16;
     }

     const size_t len = nask_utility::filesize(argv[1]);
     if (len == -1) {
	  std::cerr << "NASK : can't open " << argv[1] << std::endl;
	  return 17;
     }

     /* 入力するアセンブラ情報 */
     std::ifstream nas_file;
     nas_file.open(argv[1]);
     if ( nas_file.bad() || nas_file.fail() ) {
	  std::cerr << "NASK : can't read " << argv[1] << std::endl;
	  return 17;
     }

     /* 出力するバイナリ情報 */
     std::vector<uint8_t> binout_container;
     std::ofstream binout(argv[2], std::ios::out | std::ios::trunc | std::ios::binary);
     if ( binout.bad() || binout.fail() ) {
	  std::cerr << "NASK : can't open " << argv[2] << std::endl;
	  return 17;
     }

     // 入力の読み込みと解析，評価のループ
     process_each_assembly_line(argv, nas_file, binout_container);

     // output binaries
     for(std::vector<uint8_t>::const_iterator i = binout_container.begin(); i != binout_container.end(); ++i) {
	  binout << *i;
     }
     binout.close();

     std::cout << std::endl;

     return 0;
}
