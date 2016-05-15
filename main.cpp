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
#include "nask_defs.hpp"

int process_each_assembly_line(char** argv,
			       std::ifstream& nas_file,
			       std::vector<uint8_t>& binout_container,
			       size_t start_line = 0) {

     /* 以下，入力の読み込みと解析，評価のループ */
     long line_number = 1;
     std::string input;

     if (start_line != 0) {
	  // 開始位置まで飛ばす
	  line_number = start_line;
	  for (size_t i = 0; i < start_line - 1; i++) {
	       std::getline(nas_file, input);
	  }
     }

     for (line_number; std::getline(nas_file, input); line_number++) {

	  /* 行数チェック */
	  std::cout << line_number << ": " << std::endl;

	  /* 入力行を istream にしてトークナイザを生成 */
	  std::istrstream input_stream(input.c_str());
	  TParaTokenizer tokenizer(input_stream, &nask_utility::token_table);
	  TParaToken token;

	  while (! (token = tokenizer.Next()).IsEmpty()) {
	       if (nask_utility::is_comment_line(nask_utility::token_table, token) ||
		   nask_utility::is_line_terminated(nask_utility::token_table, token)) {
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
			 // オペコードの実装
			 //
			 if (token.AsString() == "JMP") {
			      std::cout << "eval JMP" << std::endl;
			      try {
				   const std::string label = tokenizer.Next().AsString();
				   const size_t current_line = line_number;
				   std::cout << "current line: " << current_line << std::endl;

				   // 検索用(READONLY)
				   std::ifstream nas_file_dup(argv[1], std::ios::in);

				   const size_t jump_target_line = nask_utility::get_labelpos(nas_file_dup, label + ":");
				   if (jump_target_line == -1) {
					std::cerr << "NASK : JMP target label "<< label <<  " not found " << std::endl;
					return 17;
				   } else {
					std::cout << "JMP: jump to line => " << jump_target_line << std::endl;
					int ret = process_each_assembly_line(argv,
									     nas_file,
									     binout_container,
									     jump_target_line);
				   }

			      } catch (TScriptException te) {
				   std::cerr << te << std::endl;
			      }
			      std::cout << "eval JMP end" << std::endl;

			 } else if (token.AsString() == "DB") {
			      std::cout << "eval DB" << std::endl;
			      try {
				   nask_utility::process_token_DB(tokenizer, binout_container);
			      } catch (TScriptException te) {
				   std::cerr << te << std::endl;
			      }
			      std::cout << "eval DB end" << std::endl;

			 } else if (token.AsString() == "DW") {
			      std::cout << "eval DW" << std::endl;
			      try {
				   nask_utility::process_token_DW(tokenizer, binout_container);
			      } catch (TScriptException te) {
				   std::cerr << te << std::endl;
			      }
			      std::cout << "eval DW end" << std::endl;

			 } else if (token.AsString() == "DD") {
			      std::cout << "eval DD" << std::endl;
			      try {
				   nask_utility::process_token_DD(tokenizer, binout_container);
			      } catch (TScriptException te) {
				   std::cerr << te << std::endl;
			      }
			      std::cout << "eval DD end" << std::endl;

			 } else if (token.AsString() == "RESB") {
			      std::cout << "eval RESB" << std::endl;
			      try {
				   nask_utility::process_token_RESB(tokenizer, binout_container);
			      } catch (TScriptException te) {
				   std::cerr << te << std::endl;
			      }
			      std::cout << "eval RESB end" << std::endl;
			 }

		    } else {

			 // オペコードではなくラベルの可能性を探る
			 if (nask_utility::ends_with(token.AsString(), ":")) {
			      std::cout << "coming another label" << std::endl;
			      return 0;
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

     // 各文法要素テーブルの生成
     nask_utility::token_table.AddCommentLimiter(";", "\n");
     nask_utility::token_table.AddCommentLimiter("#", "\n");

     TParaCxxOperatorTable operator_table;
     TParaObjectPrototypeTable object_prototype_table;
     TParaBuiltinFunctionTable builtin_function_table;
     TParaSymbolTable SymbolTable(&object_prototype_table, &builtin_function_table);

     /* 組み込み関数テーブルに数学関数を追加 */
     builtin_function_table.RegisterAnonymousClass(new TParaMathObject);

     /* 演算子テーブルに冪乗演算子 (**) を追加 */
     //operator_table.AddOperator(
     // 	  "**", TParaOperatorPriority("*", -1), new TParaOperatorPower()
     // 	  );
     TParaExpressionParser ExpressionParser(&operator_table);
     nask_utility::token_table.AddOperator("**");

     /* シンボルテーブルに定義済み変数 pi (円周率), e (自然対数の底), x を追加 */
     // SymbolTable.RegisterVariable("pi", TParaValue(3.141592));
     // SymbolTable.RegisterVariable("e", TParaValue(2.718281828));
     // SymbolTable.RegisterVariable("x", TParaValue((double) 0));

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
