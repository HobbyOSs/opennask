/* ParaParser.cc */
/* Created by Enomoto Sanshiro on 12 April 2001. */
/* Last updated by Enomoto Sanshiro on 12 April 2001. */


#include <iostream>
#include "ParaDefs.hh"
#include "ParaTokenizer.hh"
#include "ParaOperator.hh"
#include "ParaExpression.hh"
#include "ParaStatement.hh"
#include "ParaPackage.hh"
#include "ParaStandardLibrary.hh"
#include "ParaContainerLibrary.hh"
#include "ParaMathLibrary.hh"
#include "ParaParser.hh"

using namespace std;


TParaParser::TParaParser(void)
{
    _ObjectPrototypeTable = 0;
    _BuiltinFunctionTable = 0;
    _TokenTable = 0;
    _OperatorTable = 0;
    _StatementTable = 0;
    _Package = 0;

    _SymbolTable = 0;
    _ExpressionParser = 0;
    _StatementParser = 0;

    _IsConstructed = false;
}

TParaParser::~TParaParser()
{
    delete _StatementParser;
    delete _ExpressionParser;
    delete _SymbolTable;

    delete _Package;
    delete _StatementTable;
    delete _OperatorTable;
    delete _TokenTable;
    delete _BuiltinFunctionTable;
    delete _ObjectPrototypeTable;
}

void TParaParser::Merge(TParaParser* Source)
{
    if (! _IsConstructed) {
	Construct();
    }
    Source->Construct();

    _ObjectPrototypeTable->Merge(Source->_ObjectPrototypeTable);
    _BuiltinFunctionTable->Merge(Source->_BuiltinFunctionTable);
    _TokenTable->Merge(Source->_TokenTable);
    _OperatorTable->Merge(Source->_OperatorTable);
    _StatementTable->Merge(Source->_StatementTable);
    _Package->Merge(Source->_Package);
}

void TParaParser::Parse(istream& SourceStream) throw(TScriptException)
{
    if (! _IsConstructed) {
	Construct();
    }

    TParaTokenizer Tokenizer(SourceStream, _TokenTable);
    _Package->Parse(&Tokenizer, _StatementParser, _SymbolTable);
}

TParaValue TParaParser::Execute(const string& EntryName) throw(TScriptException)
{
    return _Package->Execute(EntryName, _SymbolTable);
}

bool TParaParser::HasEntryOf(const string& EntryName) const
{
    return (_Package->GetEntry(EntryName) != 0);
}

TParaTokenTable* TParaParser::GetTokenTable(void)
{
    if (! _IsConstructed) {
	Construct();
    }

    return _TokenTable;
}

TParaSymbolTable* TParaParser::GetSymbolTable(void)
{
    if (! _IsConstructed) {
	Construct();
    }

    return _SymbolTable;
}

TParaExpressionParser* TParaParser::GetExpressionParser(void)
{
    if (! _IsConstructed) {
	Construct();
    }

    return _ExpressionParser;
}

TParaStatementParser* TParaParser::GetStatementParser(void)
{
    if (! _IsConstructed) {
	Construct();
    }

    return _StatementParser;
}

TParaPackage* TParaParser::GetPackage(void)
{
    if (! _IsConstructed) {
	Construct();
    }

    return _Package;
}

void TParaParser::Construct(void)
{
    if (_IsConstructed) {
	return;
    }

    _ObjectPrototypeTable = CreateObjectPrototypeTable();
    _BuiltinFunctionTable = CreateBuiltinFunctionTable();
    _TokenTable = CreateTokenTable();
    _OperatorTable = CreateOperatorTable();
    _StatementTable = CreateStatementTable();
    _Package = CreatePackage();

    _SymbolTable = new TParaSymbolTable(_ObjectPrototypeTable, _BuiltinFunctionTable);
    _ExpressionParser = new TParaExpressionParser(_OperatorTable);
    _StatementParser = new TParaStatementParser(_StatementTable, _ExpressionParser);

    _IsConstructed = true;
    OnConstruct();
}

TParaObjectPrototypeTable* TParaParser::CreateObjectPrototypeTable(void)
{
    return new TParaObjectPrototypeTable();
}

TParaBuiltinFunctionTable* TParaParser::CreateBuiltinFunctionTable(void)
{
    return new TParaBuiltinFunctionTable();
}

TParaTokenTable* TParaParser::CreateTokenTable(void)
{
    return new TParaTokenTable();
}

TParaOperatorTable* TParaParser::CreateOperatorTable(void)
{
    return new TParaOperatorTable();
}

TParaStatementTable* TParaParser::CreateStatementTable(void)
{
    return new TParaStatementTable();
}

TParaPackage* TParaParser::CreatePackage(void)
{
    return new TParaPackage();
}



TParaStandardParser::TParaStandardParser(void)
{
    _argc = 0;
    _argv = 0;
}

TParaStandardParser::TParaStandardParser(int argc, char** argv)
{
    _argc = argc;
    _argv = argv;
}

TParaStandardParser::~TParaStandardParser()
{
}

TParaTokenTable* TParaStandardParser::CreateTokenTable(void)
{
    return new TParaCxxTokenTable();
}

TParaOperatorTable* TParaStandardParser::CreateOperatorTable(void)
{
    return new TParaCxxOperatorTable();
}

TParaStatementTable* TParaStandardParser::CreateStatementTable(void)
{
    return new TParaCxxStatementTable();
}

TParaPackage* TParaStandardParser::CreatePackage(void)
{
    return new TParaCxxPackage();
}

TParaObjectPrototypeTable* TParaStandardParser::CreateObjectPrototypeTable(void)
{
    TParaObjectPrototypeTable* ObjectPrototypeTable;
    ObjectPrototypeTable = TParaParser::CreateObjectPrototypeTable();

    ObjectPrototypeTable->RegisterClass("InputFile", new TParaInputFileObject);
    ObjectPrototypeTable->RegisterClass("OutputFile", new TParaOutputFileObject);
    ObjectPrototypeTable->RegisterClass("InputPipe", new TParaInputPipeObject);
    ObjectPrototypeTable->RegisterClass("OutputPipe", new TParaOutputPipeObject);

    ObjectPrototypeTable->RegisterClass("Formatter", new TParaFormatterObject);
    ObjectPrototypeTable->RegisterClass("Scanner", new TParaScannerObject);

    //... for backward compatibility ...//
    ObjectPrototypeTable->RegisterClass("Vector", new TParaVectorObject);

    return ObjectPrototypeTable;
}

TParaBuiltinFunctionTable* TParaStandardParser::CreateBuiltinFunctionTable(void)
{
    TParaBuiltinFunctionTable* BuiltinFunctionTable;
    BuiltinFunctionTable = TParaParser::CreateBuiltinFunctionTable();

    BuiltinFunctionTable->RegisterAnonymousClass(new TParaConsoleObject);
    BuiltinFunctionTable->RegisterAnonymousClass(new TParaMathObject);
    BuiltinFunctionTable->RegisterAnonymousClass(new TParaListMathObject);
    BuiltinFunctionTable->RegisterAnonymousClass(new TParaArgumentObject(_argc, _argv));
    BuiltinFunctionTable->RegisterAnonymousClass(new TParaStringObject());
    BuiltinFunctionTable->RegisterAnonymousClass(new TParaSystemObject());

    return BuiltinFunctionTable;
}
