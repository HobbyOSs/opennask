/* RepeatUntilStatement.cc */
/* Created by Enomoto Sanshiro on 24 August 2000. */
/* Last updated by Enomoto Sanshiro on 24 August 2000. */


#include "ParaStatement.hh"
#include "ParaExpression.hh"
#include "ParaOperator.hh"
#include "RepeatUntilStatement.hh"

using namespace std;


TRepeatUntilStatement::TRepeatUntilStatement(void)
{
    _ConditionExpression = 0;
    _Statement = 0;
}

TRepeatUntilStatement::~TRepeatUntilStatement()
{
    delete _ConditionExpression;
    delete _Statement;
}

TParaStatement* TRepeatUntilStatement::Clone(void)
{
    return new TRepeatUntilStatement();
}

string TRepeatUntilStatement::FirstToken(void) const
{
    return string("repeat");
}

void TRepeatUntilStatement::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false)
{
    TParaExpressionParser* ExpressionParser;
    ExpressionParser = StatementParser->ExpressionParser();

    Tokenizer->Next().MustBe("repeat");

    _Statement = StatementParser->Parse(Tokenizer, SymbolTable);

    Tokenizer->Next().MustBe("until");
    Tokenizer->Next().MustBe("(");
    _ConditionExpression = ExpressionParser->Parse(Tokenizer, SymbolTable);
    Tokenizer->Next().MustBe(")");
    Tokenizer->Next().MustBe(";");
}

TParaStatement::TExecResult TRepeatUntilStatement::Execute(TParaSymbolTable* SymbolTable) noexcept(false)
{
    TExecResult Result;

    do {
        Result = _Statement->Execute(SymbolTable);

	if (Result.ExecStatus == TParaStatement::esBreak) {
	    Result.ExecStatus = esNormal;
	    break;
	}
	if (Result.ExecStatus == TParaStatement::esContinue) {
	    Result.ExecStatus = esNormal;
	    continue;
	}
	if (Result.ExecStatus == TParaStatement::esReturn) {
	    break;
	}
    } while (! _ConditionExpression->Evaluate(SymbolTable).AsLong());

    return Result;
}
