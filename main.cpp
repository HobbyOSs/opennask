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

bool is_comment_line(TParaCxxTokenTable& token_table, TParaToken& token) {
     return token_table.IsCommentLimiter(token.AsString());
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
     TParaCxxTokenTable token_table;
     token_table.AddCommentLimiter(";", "\n");
     token_table.AddCommentLimiter("#", "\n");

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
     token_table.AddOperator("**");

     /* シンボルテーブルに定義済み変数 pi (円周率), e (自然対数の底), x を追加 */
     // SymbolTable.RegisterVariable("pi", TParaValue(3.141592));
     // SymbolTable.RegisterVariable("e", TParaValue(2.718281828));
     // SymbolTable.RegisterVariable("x", TParaValue((double) 0));

     /* 以下，入力の読み込みと解析，評価のループ */
     std::string input;
     while (nas_file && std::getline(nas_file, input)) {
	  /* 入力行を istream にしてトークナイザを生成 */
	  std::istrstream input_stream(input.c_str());
	  TParaTokenizer tokenizer(input_stream, &token_table);
	  TParaToken token;

	  while (! (token = tokenizer.Next()).IsEmpty()) {
	       std::cout << "eval: " << token.AsString() << std::endl;

	       if (is_comment_line(token_table, token)) {
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
			 std::cout << "opecode !" << std::endl;

			 //
			 // オペコードの実装
			 //
			 if (token.AsString() == "DB") {
			      // 簡単なDB命令の実装
			      std::cout << "eval DB" << std::endl;
			      try {
				   for (token = tokenizer.Next(); ; token = tokenizer.Next()) {
					if (is_comment_line(token_table, token)) {
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
			      } catch (TScriptException te) {
				   std::cerr << te << std::endl;
			      }
			      std::cout << "eval DB end" << std::endl;

			 } else if (token.AsString() == "RESB") {
			      // 簡単なRESB命令の実装
			      std::cout << "eval RESB" << std::endl;
			      try {
				   for (token = tokenizer.Next(); ; token = tokenizer.Next()) {
					if (is_comment_line(token_table, token)) {
					     break;
					} else if (token.Is(",")) {
					     continue;
					} else {
					     for (ulong l = 0; l < token.AsLong(); l++) {
						  binout_container.push_back(0x00);
					     }
					}
				   }
			      } catch (TScriptException te) {
				   std::cerr << te << std::endl;
			      }
			      std::cout << "eval RESB end" << std::endl;
			 }

		    } else {
			 std::cout << "params !" << std::endl;
		    }
	       }
	  }
     }

     // output binaries
     for(std::vector<uint8_t>::const_iterator i = binout_container.begin(); i != binout_container.end(); ++i) {
	  binout << *i;
     }
     binout.close();

     std::cout << std::endl;

     return 0;
}
