/* ParaPackage.cc */
/* Created by Enomoto Sanshiro on 1 September 1999. */
/* Last updated by Enomoto Sanshiro on 20 July 2001. */


#include <string>
#include <vector>
#include "ParaTokenizer.hh"
#include "ParaSymbolTable.hh"
#include "ParaStatement.hh"
#include "ParaFunction.hh"
#include "ParaPackage.hh"

using namespace std;


TParaPackage::TParaPackage(void)
{
    _NumberOfProcessedBareStatements = 0;
}

TParaPackage::~TParaPackage()
{
    for (unsigned i = 0; i < _EntryList.size(); i++) {
	delete _EntryList[i];
    }

    for (unsigned j = 0; j < _EntryPrototypeList.size(); j++) {
	delete _EntryPrototypeList[j];
    }

    for (unsigned k = 0; k < _BareStatementList.size(); k++) {
	delete _BareStatementList[k];
    }
}

void TParaPackage::Merge(TParaPackage* Source)
{
    for (unsigned i = 0; i < Source->_EntryList.size(); i++) {
	AddEntry(Source->_EntryList[i]->Clone());
    }
}

void TParaPackage::AddEntry(TParaPackageEntry* EntryPrototype)
{
    _EntryPrototypeList.push_back(EntryPrototype);
}

TParaPackageEntry* TParaPackage::CreateEntry(TParaTokenizer* Tokenizer)
{
    TParaPackageEntry* Entry = 0;

    vector<TParaPackageEntry*>::reverse_iterator EntryPrototype;
    for (
	EntryPrototype = _EntryPrototypeList.rbegin();
	EntryPrototype != _EntryPrototypeList.rend();
	EntryPrototype++
    ){
	if ((*EntryPrototype)->HasEntryWordsOf(Tokenizer)) {
	    Entry = (*EntryPrototype)->Clone();
	    break;
	}
    }

    return Entry;
}

void TParaPackage::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false)
{
    while (! Tokenizer->LookAhead().IsEmpty()) {
	TParaPackageEntry* Entry = CreateEntry(Tokenizer);

	if (Entry != 0) {
	    try {
		Entry->Parse(Tokenizer, StatementParser, SymbolTable);
	    }
	    catch (TScriptException &e) {
		delete Entry;
		throw;
	    }
	    _EntryList.push_back(Entry);

	    string EntryName = Entry->EntryName();
	    if (EntryName.size() > 0) {
		_EntryTable[EntryName] = Entry;
		_EntryNameList.push_back(EntryName);
	    }
	}
	else {
	    TParaStatement* Statement = StatementParser->Parse(
		Tokenizer, SymbolTable
	    );
	    _BareStatementList.push_back(Statement);
        }
    }
}

void TParaPackage::ExecuteBareStatements(TParaSymbolTable* SymbolTable) noexcept(false)
{
    TParaStatement::TExecResult Result;
    for (
	unsigned i = _NumberOfProcessedBareStatements;
	i < _BareStatementList.size();
	i++
    ){
	Result = _BareStatementList[i]->Execute(SymbolTable);
	if (Result.ExecStatus != TParaStatement::esNormal) {
	    break;
	}
    }

    _NumberOfProcessedBareStatements = _BareStatementList.size();
}

TParaValue TParaPackage::Execute(TParaSymbolTable* SymbolTable) noexcept(false)
{
    string EntryName;
    vector<TParaValue*> ArgumentList;
    return Execute(EntryName, ArgumentList, SymbolTable);
}

TParaValue TParaPackage::Execute(const string& EntryName, TParaSymbolTable* SymbolTable) noexcept(false)
{
    vector<TParaValue*> ArgumentList;
    return Execute(EntryName, ArgumentList, SymbolTable);
}

TParaValue TParaPackage::Execute(const string& EntryName, const vector<TParaValue*>& ArgumentList, TParaSymbolTable* SymbolTable) noexcept(false)
{
    if (_NumberOfProcessedBareStatements < _BareStatementList.size()) {
	ExecuteBareStatements(SymbolTable);
    }

    if (EntryName.empty()) {
	return TParaValue((long) 0);
    }
    else if (_EntryTable.count(EntryName) > 0) {
	return _EntryTable[EntryName]->Execute(ArgumentList, SymbolTable);
    }
    else {
	throw TScriptException("unknown entry: " + EntryName);
    }
}

TParaPackageEntry* TParaPackage::GetEntry(const string& EntryName)
{
    return (_EntryTable.count(EntryName) > 0) ? _EntryTable[EntryName] : 0;
}

const vector<TParaPackageEntry*>& TParaPackage::EntryList(void) const
{
    return _EntryList;
}

const vector<string>& TParaPackage::EntryNameList(void) const
{
    return _EntryNameList;
}



TParaCxxPackage::TParaCxxPackage(void)
{
    AddEntry(new TParaFunctionEntry());
    AddEntry(new TParaIncludeEntry());
}

TParaCxxPackage::~TParaCxxPackage()
{
}



TParaPackageEntry::TParaPackageEntry(const string& EntryTypeName)
{
    _EntryTypeName = EntryTypeName;
    _EntryName = "";
}

TParaPackageEntry::~TParaPackageEntry()
{
}

const string& TParaPackageEntry::EntryTypeName(void) const
{
    return _EntryTypeName;
}

const string& TParaPackageEntry::EntryName(void) const
{
    return _EntryName;
}

void TParaPackageEntry::SetEntryName(const string& EntryName)
{
    _EntryName = EntryName;
}

TParaValue TParaPackageEntry::Execute(const vector<TParaValue*>& ArgumentList, TParaSymbolTable* SymbolTable) noexcept(false)
{
    return TParaValue((long) 0);
}



TParaFunctionEntry::TParaFunctionEntry(void)
: TParaPackageEntry("Function")
{
    _Function = 0;
}

TParaFunctionEntry::~TParaFunctionEntry()
{
    delete _Function;
}

TParaPackageEntry* TParaFunctionEntry::Clone(void)
{
    return new TParaFunctionEntry();
}

bool TParaFunctionEntry::HasEntryWordsOf(TParaTokenizer* Tokenizer)
{
    TParaToken Token;
    int Index = 1;

    // return-value type declaration //
    Token = Tokenizer->LookAhead(Index++);
#if 0
    //... FIXME: SymbolTable is not accessible here
    if (! _SymbolTable->IsTypeName(Token.AsString())) {
	return false;
    }
#else
    if (! (Token.IsIdentifier() || Token.IsKeyword())) {
	return false;
    }
#endif
    while ((Token = Tokenizer->LookAhead(Index)).Is("*")) {
	Index++;
    }
        
    // function name declaration //
    Token = Tokenizer->LookAhead(Index++);
    if (! Token.IsIdentifier()) {
	return false;
    }
    
    // function parameter list declaration //
    Token = Tokenizer->LookAhead(Index++);
    if (Token.IsNot("(")) {
	return false;
    }
    
    Token = Tokenizer->LookAhead(Index++);
    if (Token.Is(")")) {
	return Tokenizer->LookAhead(Index++).IsNot(";");
    }
    if (! (Token.IsIdentifier() || (Token.IsKeyword()))) {
	// the token must be a type name
	return false;
    }
    
    while ((Token = Tokenizer->LookAhead(Index)).Is("*")) {
	Index++;
    }
    
    Token = Tokenizer->LookAhead(Index++);
    if (! Token.IsIdentifier()) {
	// the token must be a variable name
	return false;
    }

    return true;
}

void TParaFunctionEntry::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false)
{
    _Function = new TParaCxxFunction();
    try {
	_Function->Parse(Tokenizer, StatementParser, SymbolTable);
    }
    catch (TScriptException &e) {
	delete _Function;
	_Function = 0;
	throw;
    }

    string FunctionName = _Function->Name();
    long FunctionId = SymbolTable->NameToId(FunctionName);

    SymbolTable->RegisterFunction(FunctionId, _Function);
    SetEntryName(FunctionName);
}

TParaValue TParaFunctionEntry::Execute(const vector<TParaValue*>& ArgumentList, TParaSymbolTable* SymbolTable) noexcept(false)
{
    return _Function->Execute(ArgumentList, SymbolTable);
}



TParaIncludeEntry::TParaIncludeEntry(void)
: TParaPackageEntry("Include")
{
    _InputFile = 0;
}

TParaIncludeEntry::~TParaIncludeEntry()
{
    delete _InputFile;
}

TParaPackageEntry* TParaIncludeEntry::Clone(void)
{
    return new TParaIncludeEntry();
}

bool TParaIncludeEntry::HasEntryWordsOf(TParaTokenizer* Tokenizer)
{
    return Tokenizer->LookAhead().Is("include");
}

void TParaIncludeEntry::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false)
{
    Tokenizer->Next().MustBe("include");
    string FilePath = Tokenizer->Next().RemoveQuotation('"').AsString();
    Tokenizer->Next().MustBe(";");

    _InputFile = new ifstream(FilePath.c_str());
    if (! *_InputFile) {
	delete _InputFile;
	_InputFile = 0;
	throw TScriptException(
	    "include: unable to find file: " + FilePath
	);
    }

    Tokenizer->InputBuffer()->SetChildInput(*_InputFile);
}
