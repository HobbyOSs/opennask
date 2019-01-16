/* ParaStatement.cc */
/* Created by Enomoto Sanshiro on 27 August 1999. */
/* Last updated by Enomoto Sanshiro on 11 March 2002. */


#include <string>
#include <set>
#include <map>
#include "ParaTokenizer.hh"
#include "ParaObject.hh"
#include "ParaValue.hh"
#include "ParaOperator.hh"
#include "ParaExpression.hh"
#include "ParaSymbolTable.hh"
#include "ParaStatement.hh"

using namespace std;


TParaStatementTable::TParaStatementTable(void)
{
}

TParaStatementTable::~TParaStatementTable()
{
    map<string, TParaStatement*>::iterator StatementIterator;
    for (
	StatementIterator = _StatementTable.begin();
	StatementIterator != _StatementTable.end();
	StatementIterator++
    ){
	delete (*StatementIterator).second;
    }
}

void TParaStatementTable::Merge(TParaStatementTable* Source)
{
    map<string, TParaStatement*>::iterator StatementIterator;
    for (
	StatementIterator = Source->_StatementTable.begin();
	StatementIterator != Source->_StatementTable.end();
	StatementIterator++
    ){
	TParaStatement* Statement = (*StatementIterator).second;
	AddStatement(Statement->Clone());
    }
}

void TParaStatementTable::AddStatement(TParaStatement* Statement)
{
    string FirstToken = Statement->FirstToken();

    // if the same statement has already registered,
    // it will be overwritten.
    if (_StatementTable.count(FirstToken) > 0) {
	delete _StatementTable[FirstToken];
    }

    _StatementTable[FirstToken] = Statement;
}

TParaStatement* TParaStatementTable::CreateStatement(const string& FirstToken)
{
    if (_StatementTable.count(FirstToken) == 0) {
        return 0;
    }
    else {
        return _StatementTable[FirstToken]->Clone();
    }
}



TParaCxxStatementTable::TParaCxxStatementTable(void)
{
    AddStatement(new TParaComplexStatement());
    AddStatement(new TParaEmptyStatement());
    AddStatement(new TParaIfStatement());
    AddStatement(new TParaWhileStatement());
    AddStatement(new TParaForStatement());
    AddStatement(new TParaForeachStatement());
    AddStatement(new TParaBreakStatement());
    AddStatement(new TParaContinueStatement());
    AddStatement(new TParaReturnStatement());
    AddStatement(new TParaThrowStatement());
    AddStatement(new TParaTryCatchStatement());
}

TParaCxxStatementTable::~TParaCxxStatementTable()
{
}



TParaStatementParser::TParaStatementParser(TParaStatementTable* StatementTable, TParaExpressionParser* ExpressionParser)
{
    _StatementTable = StatementTable;
    _ExpressionParser = ExpressionParser;
}

TParaStatementParser::~TParaStatementParser()
{
}

TParaStatement* TParaStatementParser::Parse(TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable) noexcept(false)
{
    TParaStatement* Statement = 0;

    TParaToken Token = Tokenizer->LookAhead();
    Statement = _StatementTable->CreateStatement(Token.AsString());

    if (Statement == 0) {
        if (
	    (SymbolTable->IsTypeName(Token.AsString())) &&
	    (Tokenizer->LookAhead(2).IsNot("("))
	){
            Statement = new TParaVariableDeclarationStatement();
        }
        else {
            Statement = new TParaExpressionStatement();
        }
    }

    try {
        Statement->Parse(Tokenizer, this, SymbolTable);
    }
    catch (TScriptException& e) {
        delete Statement;
        throw e;
    }

    return Statement;
}

TParaExpressionParser* TParaStatementParser::ExpressionParser(void) const
{
    return _ExpressionParser;
}



TParaStatement::TExecResult::TExecResult(void)
: ExecStatus(esNormal)
{
}

TParaStatement::TExecResult::TExecResult(TParaValue& Value)
: ExecStatus(esNormal), ReturnValue(Value)
{
}

TParaStatement::TParaStatement(void)
{
}

TParaStatement::~TParaStatement()
{
}



TParaVariableDeclaration::TParaVariableDeclaration(const string& TypeName)
{
    _TypeName = TypeName;

    _InitialValue = 0;
    _InitializeExpression = 0;
    _ArrayLengthExpression = 0;
    _IsArray = false;
}

TParaVariableDeclaration::~TParaVariableDeclaration()
{
    if (_ConstructorArgumentList.size() > 0) {
        for (unsigned i = 0; i < _ConstructorArgumentList.size(); i++) {
            delete _ConstructorArgumentList[i];
	}
    }

    delete _ArrayLengthExpression;
    delete _InitializeExpression;
    delete _InitialValue;
}

void TParaVariableDeclaration::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false)
{
    TParaToken Token;
    TParaExpressionParser* ExpressionParser;
    ExpressionParser = StatementParser->ExpressionParser();

    if (_TypeName.empty()) {
	_TypeName = (Token = Tokenizer->Next()).AsString();
	if (! SymbolTable->IsTypeName(_TypeName)) {
	    Token.ThrowUnexpected("type name");
	}
    }

    while ((Token = Tokenizer->Next()).Is("*")) {
	_TypeName = "pointer";
    }

    if (! Token.IsIdentifier()) {
	Token.ThrowUnexpected("variable name");
    }
    _VariableName = Token.AsString();
    _VariableId = SymbolTable->NameToId(_VariableName);
    
    // array declaration //
    if (Tokenizer->LookAhead().Is("[")) {
	Tokenizer->Next();
	if (Tokenizer->LookAhead().IsNot("]")) {
	    _ArrayLengthExpression = ExpressionParser->Parse(Tokenizer, SymbolTable);
	}
	_IsArray = true;
	Tokenizer->Next().MustBe("]");
    }

    // initialization-arguments //
    if (Tokenizer->LookAhead().Is("(")) {
	_ConstructorArgumentList = ExpressionParser->ParseExpressionList(
	    Tokenizer, SymbolTable, "(", ")", ","
	);
    }

    // initialization-expression //
    if (Tokenizer->LookAhead().Is("=")) {
	Tokenizer->Next();
        _InitializeExpression = ExpressionParser->Parse(Tokenizer, SymbolTable);
    }
}

void TParaVariableDeclaration::Execute(TParaSymbolTable* SymbolTable) noexcept(false)
{
    // initialization-argument evaluation //
    vector<TParaValue*> ArgumentList;
    if (_ConstructorArgumentList.size() > 0) {
 	for (unsigned i = 0; i < _ConstructorArgumentList.size(); i++) {
	    ArgumentList.push_back(
		&_ConstructorArgumentList[i]->Evaluate(SymbolTable)
	    );
	}
    }

    // initialization-expression evaluation //
    TParaValue InitializeValue;
    long InitializeValueListLength = -1;
    if (_InitializeExpression != 0) {
	InitializeValue = _InitializeExpression->Evaluate(SymbolTable);
	if (InitializeValue.IsList()) {
	    InitializeValueListLength = InitializeValue.AsValueList().size();
	}
    }

    // array length evaluation and consistency check //
    long ArrayLength = -1;
    if (_ArrayLengthExpression != 0) {
	ArrayLength = _ArrayLengthExpression->Evaluate(SymbolTable).AsLong();
	if (
	    (InitializeValueListLength >= 0) && 
	    (ArrayLength != InitializeValueListLength)
	){
	    throw TScriptException("inconsistent initializarion list size");
	}
    }
    else if (_IsArray) {
	if (InitializeValueListLength < 0) {
	    throw TScriptException("array size is expected");
	}
	ArrayLength = InitializeValueListLength;
    }

    // variable creation //
    TParaValue* Variable = 0;
    TParaValue* ArrayValue = 0;
    if (! _IsArray) {
	Variable = SymbolTable->CreateObject(_TypeName);
    }
    else {	
	ArrayValue = SymbolTable->CreateObject(_TypeName, ArrayLength);
	for (int i = 0; i < ArrayLength; i++) {
	    ArrayValue[i].SetLeftValueFlag();
	    ArrayValue[i].SetName(TParaValue(ArrayValue + i).AsString());
	}
	Variable = new TParaValue(ArrayValue);
	Variable->SetArrayFlag();
    }
    Variable->SetName(_VariableName);

    // initialization-argument processing //
    try {
	if (Variable->IsObject()) {
	    Variable->AsObject()->Construct(_TypeName, ArgumentList);
	}
	else {
	    if (ArgumentList.size() > 1) {
		throw TScriptException("too many initial-value arguments");
	    }
	    else if (! ArgumentList.empty()) {
		if (! _IsArray) {
		    Variable->Assign(*ArgumentList[0]);
		}
		else {
		    for (int i = 0; i < ArrayLength; i++) {
			ArrayValue[i].Assign(*ArgumentList[0]);
		    }
		}
	    }
	}
	
	// initialization-expression processing //
	if (_InitializeExpression != 0) {
	    if (! _IsArray) {
		Variable->Assign(InitializeValue);
	    }
	    else {
		for (int i = 0; i < ArrayLength; i++) {
		    ArrayValue[i].Assign(InitializeValue.AsValueList()[i]);
		}
	    }
	}
	
	// symbol registration: only if declaration is successful //
	SymbolTable->RegisterVariable(_VariableId, Variable);
    }
    catch (TScriptException &e) {
	delete Variable;
	delete[] ArrayValue;
	throw;
    }
}

long TParaVariableDeclaration::VariableId(void) const
{
    return _VariableId;
}



TParaVariableDeclarationStatement::TParaVariableDeclarationStatement(void)
{
}

TParaVariableDeclarationStatement::~TParaVariableDeclarationStatement()
{
    for (unsigned i = 0; i < _VariableDeclarationList.size(); i++) {
	delete _VariableDeclarationList[i];
    }
}

TParaStatement* TParaVariableDeclarationStatement::Clone(void)
{
    return new TParaVariableDeclarationStatement();
}

string TParaVariableDeclarationStatement::FirstToken(void) const
{
    return string("");
}

void TParaVariableDeclarationStatement::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false)
{
    TParaToken Token = Tokenizer->Next();
    _Position = Token.Position();
    _TypeName = Token.AsString();

    if (! SymbolTable->IsTypeName(_TypeName)) {
        Token.ThrowUnexpected("type name");
    }

    try {
	TParaVariableDeclaration* VariableDeclaration;
	do {
	    VariableDeclaration = new TParaVariableDeclaration(_TypeName);
	    VariableDeclaration->Parse(Tokenizer, StatementParser, SymbolTable);
	    _VariableDeclarationList.push_back(VariableDeclaration);
	} while ((Token = Tokenizer->Next()).Is(","));

	Token.MustBe(";");
    }
    catch (TScriptException &e) {
	for (unsigned i = 0; i < _VariableDeclarationList.size(); i++) {
	    delete _VariableDeclarationList[i];
	}
	_VariableDeclarationList.erase(
	    _VariableDeclarationList.begin(),
	    _VariableDeclarationList.end()
	);
	throw;
    }
}

TParaStatement::TExecResult TParaVariableDeclarationStatement::Execute(TParaSymbolTable* SymbolTable) noexcept(false)
{
    try {
	for (unsigned i = 0; i < _VariableDeclarationList.size(); i++) {
	    _VariableDeclarationList[i]->Execute(SymbolTable);
	}
    }
    catch (TScriptException &e) {
	throw TScriptException(_Position + e.Message());
    }

    return TExecResult();
}



TParaExpressionStatement::TParaExpressionStatement(void)
{
    _Expression = 0;
}

TParaExpressionStatement::~TParaExpressionStatement()
{
    delete _Expression;
}

TParaStatement* TParaExpressionStatement::Clone(void)
{
    return new TParaExpressionStatement();
}

string TParaExpressionStatement::FirstToken(void) const
{
    return string("");
}

void TParaExpressionStatement::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false)
{
    TParaExpressionParser* ExpressionParser;
    ExpressionParser = StatementParser->ExpressionParser();

    _Expression = ExpressionParser->Parse(Tokenizer, SymbolTable);

    TParaToken Token = Tokenizer->Next();
    if (Token.IsNot(";")) {
	throw TScriptException(
	    _Expression->Position() + 
	    "syntax error near '" + Token.AsString() + "'"
	);
    }
}



TParaEmptyStatement::TParaEmptyStatement(void)
{
}

TParaEmptyStatement::~TParaEmptyStatement()
{
}

TParaStatement* TParaEmptyStatement::Clone(void)
{
    return new TParaEmptyStatement();
}

string TParaEmptyStatement::FirstToken(void) const
{
    return string(";");
}

void TParaEmptyStatement::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false)
{
    Tokenizer->Next().MustBe(";");
}

TParaStatement::TExecResult TParaEmptyStatement::Execute(TParaSymbolTable* SymbolTable) noexcept(false)
{
    return TExecResult();
}


TParaStatement::TExecResult TParaExpressionStatement::Execute(TParaSymbolTable* SymbolTable) noexcept(false)
{
    return TExecResult(_Expression->Evaluate(SymbolTable));
}



TParaComplexStatement::TParaComplexStatement(void)
{
}

TParaComplexStatement::~TParaComplexStatement()
{
    vector<TParaStatement*>::iterator Statement;
    for (
	 Statement = _StatementList.begin();
	 Statement != _StatementList.end();
	 Statement++
    ){
       delete *Statement;
    } 
}

TParaStatement* TParaComplexStatement::Clone(void)
{
    return new TParaComplexStatement();
}

string TParaComplexStatement::FirstToken(void) const
{
    return string("{");
}

void TParaComplexStatement::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false)
{
    Tokenizer->Next().MustBe("{");
    
    TParaToken Token;
    TParaStatement* Statement = 0;
    while ((Token = Tokenizer->Next()).IsNot("}")) {
        if (Token.IsEmpty()) {
	    Token.ThrowUnexpected("}");
	}
	else {
	    Tokenizer->Unget(Token);
	}

        Statement = StatementParser->Parse(Tokenizer, SymbolTable);
	_StatementList.push_back(Statement);
    }
}

TParaStatement::TExecResult TParaComplexStatement::Execute(TParaSymbolTable* SymbolTable) noexcept(false)
{
    SymbolTable->EnterBlock();

    TExecResult Result;
    try {
	vector<TParaStatement*>::const_iterator Statement;
	for (
	    Statement = _StatementList.begin();
	    Statement != _StatementList.end();
	    Statement++
	){
	    Result = (*Statement)->Execute(SymbolTable);
	    if (Result.ExecStatus != TParaStatement::esNormal) {
		break;
	    }
	}
    }
    catch (TScriptException &e) {
	SymbolTable->ExitBlock();
	throw;
    }

    SymbolTable->ExitBlock();

    return Result;
}



TParaIfStatement::TParaIfStatement(void)
{
    _ConditionExpression = 0;
    _TrueStatement = 0;
    _FalseStatement = 0;
}

TParaIfStatement::~TParaIfStatement()
{
    delete _ConditionExpression;
    delete _TrueStatement;
    delete _FalseStatement;
}

TParaStatement* TParaIfStatement::Clone(void)
{
    return new TParaIfStatement();
}

string TParaIfStatement::FirstToken(void) const
{
    return string("if");
}

void TParaIfStatement::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false)
{
    TParaExpressionParser* ExpressionParser;
    ExpressionParser = StatementParser->ExpressionParser();

    Tokenizer->Next().MustBe("if");

    Tokenizer->Next().MustBe("(");
    _ConditionExpression = ExpressionParser->Parse(Tokenizer, SymbolTable);
    Tokenizer->Next().MustBe(")");

    _TrueStatement = StatementParser->Parse(Tokenizer, SymbolTable);

    TParaToken Token = Tokenizer->Next();
    if (Token.Is("else")) {
        _FalseStatement = StatementParser->Parse(Tokenizer, SymbolTable);
    }
    else {
        Tokenizer->Unget(Token);
    }
}

TParaStatement::TExecResult TParaIfStatement::Execute(TParaSymbolTable* SymbolTable) noexcept(false)
{
    TExecResult Result;

    if (_ConditionExpression->Evaluate(SymbolTable).AsBool()) {
        Result = _TrueStatement->Execute(SymbolTable);
    }
    else {
        if (_FalseStatement != 0) {
	    Result = _FalseStatement->Execute(SymbolTable);
	}
    }

    return Result;
}



TParaWhileStatement::TParaWhileStatement(void)
{
    _ConditionExpression = 0;
    _Statement = 0;
}

TParaWhileStatement::~TParaWhileStatement()
{
    delete _ConditionExpression;
    delete _Statement;
}

TParaStatement* TParaWhileStatement::Clone(void)
{
    return new TParaWhileStatement();
}

string TParaWhileStatement::FirstToken(void) const
{
    return string("while");
}

void TParaWhileStatement::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false)
{
    TParaExpressionParser* ExpressionParser;
    ExpressionParser = StatementParser->ExpressionParser();

    Tokenizer->Next().MustBe("while");

    Tokenizer->Next().MustBe("(");
    _ConditionExpression = ExpressionParser->Parse(Tokenizer, SymbolTable);
    Tokenizer->Next().MustBe(")");

    _Statement = StatementParser->Parse(Tokenizer, SymbolTable);
}

TParaStatement::TExecResult TParaWhileStatement::Execute(TParaSymbolTable* SymbolTable) noexcept(false)
{
    TExecResult Result;

    while (_ConditionExpression->Evaluate(SymbolTable).AsBool()) {
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
    }

    return Result;
}



TParaForStatement::TParaForStatement(void)
{
    _InitializeStatement = 0;
    _ConditionExpression = 0;
    _IncrementExpression = 0;
    _Statement = 0;
}

TParaForStatement::~TParaForStatement()
{
    delete _InitializeStatement;
    delete _ConditionExpression;
    delete _IncrementExpression;
    delete _Statement;
}

TParaStatement* TParaForStatement::Clone(void)
{
    return new TParaForStatement();
}

string TParaForStatement::FirstToken(void) const
{
    return string("for");
}

void TParaForStatement::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false)
{
    TParaExpressionParser* ExpressionParser;
    ExpressionParser = StatementParser->ExpressionParser();

    Tokenizer->Next().MustBe("for");

    Tokenizer->Next().MustBe("(");
    _InitializeStatement = StatementParser->Parse(Tokenizer, SymbolTable);
    _ConditionExpression = ExpressionParser->Parse(Tokenizer, SymbolTable);
    Tokenizer->Next().MustBe(";");
    _IncrementExpression = ExpressionParser->Parse(Tokenizer, SymbolTable);
    Tokenizer->Next().MustBe(")");

    _Statement = StatementParser->Parse(Tokenizer, SymbolTable);
}

TParaStatement::TExecResult TParaForStatement::Execute(TParaSymbolTable* SymbolTable) noexcept(false)
{
    TExecResult Result;

    SymbolTable->EnterBlock();
    
    try {
	_InitializeStatement->Execute(SymbolTable);
	while (_ConditionExpression->Evaluate(SymbolTable).AsBool()) {
	    Result = _Statement->Execute(SymbolTable);
	    
	    if (Result.ExecStatus == TParaStatement::esBreak) {
		Result.ExecStatus = esNormal;
		break;
	    }
	    if (Result.ExecStatus == TParaStatement::esReturn) {
		break;
	    }
	    if (Result.ExecStatus == TParaStatement::esContinue) {
		Result.ExecStatus = esNormal;
	    }
	    
	    _IncrementExpression->Evaluate(SymbolTable);
	}
    }
    catch (TScriptException &e) {
	SymbolTable->ExitBlock();
	throw;
    }

    SymbolTable->ExitBlock();
    
    return Result;
}



TParaForeachStatement::TParaForeachStatement(void)
{
    _VariableExpression = 0;
    _KeyExpression = 0;
    _IndexExpression = 0;
    _ListExpression = 0;
    _Statement = 0;
}

TParaForeachStatement::~TParaForeachStatement()
{
    delete _VariableExpression;
    delete _KeyExpression;
    delete _IndexExpression;
    delete _ListExpression;
    delete _Statement;
}

TParaStatement* TParaForeachStatement::Clone(void)
{
    return new TParaForeachStatement();
}

string TParaForeachStatement::FirstToken(void) const
{
    return string("foreach");
}

void TParaForeachStatement::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false)
{
    TParaExpressionParser* ExpressionParser;
    ExpressionParser = StatementParser->ExpressionParser();

    Tokenizer->Next().MustBe("foreach");

    Tokenizer->Next().MustBe("(");
    if (Tokenizer->LookAhead().IsNot(";")) {
	_VariableExpression = ExpressionParser->Parse(Tokenizer, SymbolTable);
    }
    if (Tokenizer->LookAhead().IsNot(";")) {
	Tokenizer->Next().MustBe(",");
	_KeyExpression = ExpressionParser->Parse(Tokenizer, SymbolTable);
    }
    if (Tokenizer->LookAhead().IsNot(";")) {
	Tokenizer->Next().MustBe(",");
	_IndexExpression = ExpressionParser->Parse(Tokenizer, SymbolTable);
    }
    Tokenizer->Next().MustBe(";");
    _ListExpression = ExpressionParser->Parse(Tokenizer, SymbolTable);
    Tokenizer->Next().MustBe(")");

    _Statement = StatementParser->Parse(Tokenizer, SymbolTable);
}

TParaStatement::TExecResult TParaForeachStatement::Execute(TParaSymbolTable* SymbolTable) noexcept(false)
{
    TExecResult Result;

    SymbolTable->EnterBlock();
    
    try {
	TParaValue& ListExpressionValue = _ListExpression->Evaluate(SymbolTable);
	if (! ListExpressionValue.IsList()) {
	    throw TScriptException(
		_ListExpression->Position() + 
		"foreach: list value is expected"
	    );
	}
	TParaListValue& ListValue = ListExpressionValue.AsList();

	TParaValue* Variable = 0;
	if (_VariableExpression) {
	    Variable = &(_VariableExpression->Evaluate(SymbolTable));
	    if (! Variable->IsLeftValue()) {
		throw TScriptException(
		    _VariableExpression->Position() + 
		    "foreach: L-value is expected"
		);
	    }
	}
	
	TParaValue* Key = 0;
	if (_KeyExpression && ListValue.HasKeyIndex()) {
	    Key = &(_KeyExpression->Evaluate(SymbolTable));
	    if (! Key->IsLeftValue()) {
		throw TScriptException(
		    _IndexExpression->Position() + 
		    "foreach: L-value is expected"
		);
	    }
	}
	
	TParaValue* Index = 0;
	if (_IndexExpression) {
	    Index = &(_IndexExpression->Evaluate(SymbolTable));
	    if (! Index->IsLeftValue()) {
		throw TScriptException(
		    _IndexExpression->Position() + 
		    "foreach: L-value is expected"
		);
	    }
	}
	
	for (unsigned i = 0; i < ListValue.ListSize(); i++) {
	    try {
		if (Variable) {
		    Variable->Assign(ListValue.ValueOf(i));
		}
		if (Key) {
		    Key->Assign(TParaValue(ListValue.KeyOf(i)));
		}
		if (Index) {
		    Index->Assign(TParaValue((long) i));
		}
	    }
	    catch (TScriptException &e) {
		throw TScriptException(
		    _ListExpression->Position() + e.Message()
		);
	    }
	    
	    Result = _Statement->Execute(SymbolTable);
	    
	    if (Result.ExecStatus == TParaStatement::esBreak) {
		Result.ExecStatus = esNormal;
		break;
	    }
	    if (Result.ExecStatus == TParaStatement::esReturn) {
		break;
	    }
	    if (Result.ExecStatus == TParaStatement::esContinue) {
		Result.ExecStatus = esNormal;
	    }
	}
    }
    catch (TScriptException &e) {
	SymbolTable->ExitBlock();
	throw;
    }

    SymbolTable->ExitBlock();
    
    return Result;
}



TParaBreakStatement::TParaBreakStatement(void)
{
}

TParaBreakStatement::~TParaBreakStatement()
{
}

TParaStatement* TParaBreakStatement::Clone(void)
{
    return new TParaBreakStatement();
}

string TParaBreakStatement::FirstToken(void) const
{
    return string("break");
}

void TParaBreakStatement::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false)
{
    Tokenizer->Next().MustBe("break");
    Tokenizer->Next().MustBe(";");
}

TParaStatement::TExecResult TParaBreakStatement::Execute(TParaSymbolTable* SymbolTable) noexcept(false)
{
    TExecResult Result;
    Result.ExecStatus = TParaStatement::esBreak;

    return Result;
}



TParaContinueStatement::TParaContinueStatement(void)
{
}

TParaContinueStatement::~TParaContinueStatement()
{
}

TParaStatement* TParaContinueStatement::Clone(void)
{
    return new TParaContinueStatement();
}

string TParaContinueStatement::FirstToken(void) const
{
    return string("continue");
}

void TParaContinueStatement::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false)
{
    Tokenizer->Next().MustBe("continue");
    Tokenizer->Next().MustBe(";");
}

TParaStatement::TExecResult TParaContinueStatement::Execute(TParaSymbolTable* SymbolTable) noexcept(false)
{
    TExecResult Result;
    Result.ExecStatus = TParaStatement::esContinue;

    return Result;
}



TParaReturnStatement::TParaReturnStatement(void)
{
    _Expression = 0;
}

TParaReturnStatement::~TParaReturnStatement()
{
    delete _Expression;
}

TParaStatement* TParaReturnStatement::Clone(void)
{
    return new TParaReturnStatement();
}

string TParaReturnStatement::FirstToken(void) const
{
    return string("return");
}

void TParaReturnStatement::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false)
{
    TParaExpressionParser* ExpressionParser;
    ExpressionParser = StatementParser->ExpressionParser();

    Tokenizer->Next().MustBe("return");

    TParaToken Token = Tokenizer->Next();
    if (Token.IsNot(";")) {
        Tokenizer->Unget(Token);
        _Expression = ExpressionParser->Parse(Tokenizer, SymbolTable);
        Tokenizer->Next().MustBe(";");
    }
}

TParaStatement::TExecResult TParaReturnStatement::Execute(TParaSymbolTable* SymbolTable) noexcept(false)
{
    TExecResult Result;

    if (_Expression != 0) {
        Result.ReturnValue = _Expression->Evaluate(SymbolTable);
    }

    Result.ExecStatus = TParaStatement::esReturn;

    return Result;
}



TParaThrowStatement::TParaThrowStatement(void)
{
    _ExceptionExpression = 0;
}

TParaThrowStatement::~TParaThrowStatement()
{
    delete _ExceptionExpression;
}

TParaStatement* TParaThrowStatement::Clone(void)
{
    return new TParaThrowStatement();
}

string TParaThrowStatement::FirstToken(void) const
{
    return string("throw");
}

void TParaThrowStatement::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false)
{
    TParaExpressionParser* ExpressionParser;
    ExpressionParser = StatementParser->ExpressionParser();

    Tokenizer->Next().MustBe("throw");

    TParaToken Token = Tokenizer->Next();
    if (Token.IsNot(";")) {
        Tokenizer->Unget(Token);
        _ExceptionExpression = ExpressionParser->Parse(Tokenizer, SymbolTable);
        Tokenizer->Next().MustBe(";");
    }
}

TParaStatement::TExecResult TParaThrowStatement::Execute(TParaSymbolTable* SymbolTable) noexcept(false)
{
    if (_ExceptionExpression != 0) {        
	throw TScriptException(
	    _ExceptionExpression->Evaluate(SymbolTable).AsString()
	);
    }

    return TExecResult();
}



TParaTryCatchStatement::TParaTryCatchStatement(void)
{
    _TryStatement = 0;
    _ArgumentDeclaration = 0;
    _CatchStatement = 0;
}

TParaTryCatchStatement::~TParaTryCatchStatement()
{
    delete _TryStatement;
    delete _ArgumentDeclaration;
    delete _CatchStatement;
}

TParaStatement* TParaTryCatchStatement::Clone(void)
{
    return new TParaTryCatchStatement();
}

string TParaTryCatchStatement::FirstToken(void) const
{
    return string("try");
}

void TParaTryCatchStatement::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false)
{
    Tokenizer->Next().MustBe("try");
    _TryStatement = StatementParser->Parse(Tokenizer, SymbolTable);

    if (Tokenizer->LookAhead().Is("catch")) {
	Tokenizer->Next();
	if (Tokenizer->LookAhead().Is("(")) {
	    Tokenizer->Next().MustBe("(");
            _ArgumentDeclaration = new TParaVariableDeclaration();
	    try {
		_ArgumentDeclaration->Parse(Tokenizer, StatementParser, SymbolTable);
		Tokenizer->Next().MustBe(")");
	    }
	    catch (TScriptException &e) {
		delete _ArgumentDeclaration;
		_ArgumentDeclaration = 0;
		throw;
	    }
	}

	_CatchStatement = StatementParser->Parse(Tokenizer, SymbolTable);
    }
}

TParaStatement::TExecResult TParaTryCatchStatement::Execute(TParaSymbolTable* SymbolTable) noexcept(false)
{
    TExecResult Result;

    try {
        Result = _TryStatement->Execute(SymbolTable);
    }
    catch (TScriptException &e) {
	SymbolTable->EnterBlock();

	try {
	    if (_ArgumentDeclaration) {
		TParaValue Argument(e.Message());
		_ArgumentDeclaration->Execute(SymbolTable);
		long VariableId = _ArgumentDeclaration->VariableId();
		SymbolTable->GetVariable(VariableId)->Assign(Argument);
	    }
	    if (_CatchStatement) {
		_CatchStatement->Execute(SymbolTable);
	    }
	}
	catch (TScriptException &e) {
	    SymbolTable->ExitBlock();
	    throw;
	}

	SymbolTable->ExitBlock();
    }
    
    return Result;
}
