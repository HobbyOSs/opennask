#include <iostream>
#include <strstream>
#include "ParaTokenizer.hh"
#include "ParaOperator.hh"
#include "ParaExpression.hh"
#include "ParaSymbolTable.hh"
#include "ParaMathLibrary.hh"

int main(int argc, char** argv)
{
/* 各文法要素テーブルの生成 */
     TParaCxxTokenTable TokenTable;
     TParaCxxOperatorTable OperatorTable;
     TParaObjectPrototypeTable ObjectPrototypeTable;
     TParaBuiltinFunctionTable BuiltinFunctionTable;
     TParaSymbolTable SymbolTable(&ObjectPrototypeTable, &BuiltinFunctionTable);

     /* 組み込み関数テーブルに数学関数を追加 */
     BuiltinFunctionTable.RegisterAnonymousClass(new TParaMathObject);

     /* 演算子テーブルに冪乗演算子 (**) を追加 */
     //OperatorTable.AddOperator(
     // 	  "**", TParaOperatorPriority("*", -1), new TParaOperatorPower()
     // 	  );
     TParaExpressionParser ExpressionParser(&OperatorTable);
     TokenTable.AddOperator("**");

     /* シンボルテーブルに定義済み変数 pi (円周率), e (自然対数の底), x を追加 */
     SymbolTable.RegisterVariable("pi", TParaValue(3.141592));
     SymbolTable.RegisterVariable("e", TParaValue(2.718281828));
     SymbolTable.RegisterVariable("x", TParaValue((double) 0));

     /* 以下，入力の読み込みと解析，評価のループ */
     std::string Input;
     while (std::cout << "> ", getline(std::cin, Input, '\n')) {
	  /* 入力行を istream にしてトークナイザを生成 */
	  std::istrstream InputStream(Input.c_str());
	  TParaTokenizer Tokenizer(InputStream, &TokenTable);

	  TParaExpression* Expression = 0;
	  try {
	       /* トークナイザを渡して，入力を解析 */
	       Expression = ExpressionParser.Parse(&Tokenizer, &SymbolTable);

	       /* 解析に成功したら，評価 */
	       TParaValue Value = Expression->Evaluate(&SymbolTable);
	       std::cout << Value.AsString() << std::endl;
	  }
	  catch (TScriptException &e) {
	       std::cerr << "ERROR: " << e << std::endl;
	  }

	  /* Parse() で生成された式オブジェクトは毎回手動で delete する */
	  delete Expression;
     }

     std::cout << std::endl;

     return 0;
}
