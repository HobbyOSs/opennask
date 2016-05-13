#include <iostream>
#include <strstream>
#include "ParaTokenizer.hh"
#include "ParaOperator.hh"
#include "ParaExpression.hh"
#include "ParaSymbolTable.hh"
#include "ParaMathLibrary.hh"
#include "nask_utility.hpp"

int main(int argc, char** argv)
{

     if (argc < 2 || argc > 4) {
	  std::cerr << "usage : >opennask source [object/binary] [list]" << std::endl;
	  return 16;
     }

     const size_t len = nask_utility::filesize(argv[1]);
     if (len == -1) {
	  std::cerr << "NASK : can't open " << argv[1] << std::endl;
	  return 17;
     }

     // open .nas file
     std::ifstream nas_file;
     nas_file.open(argv[1]);
     if ( nas_file.bad() || nas_file.fail() ) {
	  std::cerr << "NASK : can't read " << argv[1] << std::endl;
	  return 17;
     }

     // 各文法要素テーブルの生成
     TParaCxxTokenTable token_table;
     token_table.AddOperator(";");

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

	       if (token.Is(";") || token.Is("#")) {
		    std::cout << "it's comments" << std::endl;
		    break;
	       } else if (token.Is(",")) {
		    continue;
	       } else {
		    std::cout << "opecode or imm" << std::endl;
	       }
	  }
     }

     std::cout << std::endl;

     return 0;
}
