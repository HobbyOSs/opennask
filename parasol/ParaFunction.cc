/* ParaFunction.cc */
/* Created by Enomoto Sanshiro on 1 September 1999. */
/* Last updated by Enomoto Sanshiro on 12 April 2000. */


#include <string>
#include <vector>
#include "ParaTokenizer.hh"
#include "ParaExpression.hh"
#include "ParaSymbolTable.hh"
#include "ParaStatement.hh"
#include "ParaFunction.hh"

using namespace std;


TParaFunction::TParaFunction(void)
{
    _Statement = new TParaEmptyStatement();
    _ReturnValue = new TParaValue();
}

TParaFunction::~TParaFunction()
{
    for (unsigned i = 0; i < _ArgumentDeclarationList.size(); i++) {
        delete _ArgumentDeclarationList[i];
    }
    
    delete _ReturnValue;
    delete _Statement;
}

TParaValue TParaFunction::Execute(const vector<TParaValue*>& ArgumentList, TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    SymbolTable->EnterBlock();
    
    TParaStatement::TExecResult Result;
    try {
	ProcessArguments(ArgumentList, SymbolTable);
	Result = _Statement->Execute(SymbolTable); 
    }
    catch (TScriptException &e) {
	SymbolTable->ExitBlock();
	throw;
    }

    SymbolTable->ExitBlock();

    try {
	_ReturnValue->Assign(Result.ReturnValue);
    }
    catch (TScriptException &e) {
	throw TScriptException(
	    _Name + "(): bad return-value type: " + e.Message()
	);
    }
    
    return *_ReturnValue;
}

void TParaFunction::ProcessArguments(const vector<TParaValue*>& ArgumentList, TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    if (ArgumentList.size() > _ArgumentDeclarationList.size()) {
        throw TScriptException(_Name + "(): too many aguments");
    }

    for (unsigned i = 0; i < _ArgumentDeclarationList.size(); i++) {
        _ArgumentDeclarationList[i]->Execute(SymbolTable);

	if (i < ArgumentList.size()) {
	    long VariableId = _ArgumentDeclarationList[i]->VariableId();
	    SymbolTable->GetVariable(VariableId)->Assign(*ArgumentList[i]);
	}
    }
}

string TParaFunction::Name(void)
{
    return _Name;
}

void TParaFunction::SetName(const string& Name)
{
    _Name = Name;
}

void TParaFunction::SetReturnValue(TParaValue* ReturnValue)
{
    delete _ReturnValue;
    _ReturnValue = ReturnValue;
}

void TParaFunction::AddArgumentDeclaration(TParaVariableDeclaration* ArgumentDeclaration)
{
    _ArgumentDeclarationList.push_back(ArgumentDeclaration);
}

void TParaFunction::SetStatement(TParaStatement* Statement)
{
    delete _Statement;
    _Statement = Statement;
}



TParaCxxFunction::TParaCxxFunction(void)
{
}

TParaCxxFunction::~TParaCxxFunction()
{
}

void TParaCxxFunction::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    TParaToken Token = Tokenizer->Next();    
    TParaValue* ReturnValue = SymbolTable->CreateObject(Token.AsString());
    if (ReturnValue == 0) {
	Token.ThrowUnexpected("type name");
    }
    while ((Token = Tokenizer->Next()).Is("*")) {
	delete ReturnValue;
	ReturnValue = new TParaValue((TParaValue*) 0);
    }
    SetReturnValue(ReturnValue);

    if (! Token.IsIdentifier()) {
        Token.ThrowUnexpected("function name");
    }
    string FunctionName = Token.AsString();
    SetName(FunctionName);

    ParseArgumentDeclaration(Tokenizer, StatementParser, SymbolTable);
    
    TParaStatement* Statement = StatementParser->Parse(Tokenizer, SymbolTable);
    SetStatement(Statement);
}

void TParaCxxFunction::ParseArgumentDeclaration(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    Tokenizer->Next().MustBe("(");

    TParaToken Token;
    if (Tokenizer->LookAhead().Is(")")) {
	Token = Tokenizer->Next();
    }
    else {
	TParaVariableDeclaration* VariableDeclaration;
	do {
	    try {
		VariableDeclaration = new TParaVariableDeclaration();
		VariableDeclaration->Parse(Tokenizer, StatementParser, SymbolTable);
	    }
	    catch (TScriptException &e) {
		delete VariableDeclaration;
		throw;
	    }
            AddArgumentDeclaration(VariableDeclaration);
	} while ((Token = Tokenizer->Next()).Is(","));
    }

    Token.MustBe(")");
}
