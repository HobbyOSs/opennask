/* ParaOperator.cc */
/* Created by Enomoto Sanshiro on 8 July 1999. */
/* Last updated by Enomoto Sanshiro on 3 May 2002. */


#include <string>
#include <cmath>
#include "ParaObject.hh"
#include "ParaValue.hh"
#include "ParaTokenizer.hh"
#include "ParaExpression.hh"
#include "ParaOperator.hh"

using namespace std;


TParaOperatorPriority::TParaOperatorPriority(int PriorityRank)
{
    _PriorityRank = PriorityRank;
    _BaseOperatorSymbol = "";
    _PriorityOffset = 0;
}

TParaOperatorPriority::TParaOperatorPriority(const string& BaseOperatorSymbol, int PriorityOffset, int TemporaryPriorityRank)
{
    _PriorityRank = TemporaryPriorityRank;
    _BaseOperatorSymbol = BaseOperatorSymbol;
    _PriorityOffset = PriorityOffset;
}

TParaOperatorPriority::TParaOperatorPriority(const TParaOperatorPriority& Priority)
{
    _PriorityRank = Priority._PriorityRank;
    _BaseOperatorSymbol = Priority._BaseOperatorSymbol;
    _PriorityOffset = Priority._PriorityOffset;
}

TParaOperatorPriority::~TParaOperatorPriority()
{
}

TParaOperatorPriority& TParaOperatorPriority::operator=(const TParaOperatorPriority& Priority)
{
    _PriorityRank = Priority._PriorityRank;
    _BaseOperatorSymbol = Priority._BaseOperatorSymbol;
    _PriorityOffset = Priority._PriorityOffset;

    return *this;
}

void TParaOperatorPriority::SetPriorityRank(int PriorityRank)
{
    _PriorityRank = PriorityRank;
}

int TParaOperatorPriority::PriorityRank(void) const
{
    return _PriorityRank;
}

const string& TParaOperatorPriority::BaseOperatorSymbol(void) const
{
    return _BaseOperatorSymbol;
}

int TParaOperatorPriority::PriorityOffset(void) const
{
    return _PriorityOffset;
}



TParaOperatorTable::TParaOperatorTable(void)
{
    _HighestPriorityRank = 1;
    _LowestPriorityRank = 0;
}

TParaOperatorTable::~TParaOperatorTable()
{
    map<string, TParaOperator*>::iterator OperatorEntry;
    for (
	OperatorEntry = _OperatorTable.begin();
	OperatorEntry != _OperatorTable.end();
	OperatorEntry++
    ){
	delete (*OperatorEntry).second;
    }

    for (
	OperatorEntry = _PrepositionalOperatorTable.begin();
	OperatorEntry != _PrepositionalOperatorTable.end();
	OperatorEntry++
    ){
	delete (*OperatorEntry).second;
    }

    for (
	OperatorEntry = _PostpositionalOperatorTable.begin();
	OperatorEntry != _PostpositionalOperatorTable.end();
	OperatorEntry++
    ){
	delete (*OperatorEntry).second;
    }

    for (
	OperatorEntry = _ElementaryOperatorTable.begin();
	OperatorEntry != _ElementaryOperatorTable.end();
	OperatorEntry++
    ){
	delete (*OperatorEntry).second;
    }
}

void TParaOperatorTable::Merge(TParaOperatorTable* Source)
{
    map<string, TParaOperator*>::iterator OperatorTableIterator;
    for (
	OperatorTableIterator = Source->_OperatorTable.begin();
	OperatorTableIterator != Source->_OperatorTable.end();
	OperatorTableIterator++
    ){
	string Symbol = (*OperatorTableIterator).first;
	const TParaOperatorPriority& Priority = Source->PriorityOf(Symbol);
	TParaOperator* Operator = (*OperatorTableIterator).second;
	AddOperator(Operator->Clone(), Priority);
    }

    for (
	OperatorTableIterator = Source->_PrepositionalOperatorTable.begin();
	OperatorTableIterator != Source->_PrepositionalOperatorTable.end();
	OperatorTableIterator++
    ){
	string Symbol = (*OperatorTableIterator).first;
	TParaOperator* Operator = (*OperatorTableIterator).second;
	AddPrepositionalOperator(Operator->Clone());
    }

    for (
	OperatorTableIterator = Source->_PostpositionalOperatorTable.begin();
	OperatorTableIterator != Source->_PostpositionalOperatorTable.end();
	OperatorTableIterator++
    ){
	string Symbol = (*OperatorTableIterator).first;
	TParaOperator* Operator = (*OperatorTableIterator).second;
	AddPostpositionalOperator(Operator->Clone());
    }

    for (
	OperatorTableIterator = Source->_ElementaryOperatorTable.begin();
	OperatorTableIterator != Source->_ElementaryOperatorTable.end();
	OperatorTableIterator++
    ){
	string Symbol = (*OperatorTableIterator).first;
	TParaOperator* Operator = (*OperatorTableIterator).second;
	AddElementaryOperator(Operator->Clone());
    }
}

void TParaOperatorTable::AddOperator(TParaOperator* Operator, int PriorityRank)
{
    AddOperator(Operator, TParaOperatorPriority(PriorityRank));
}

void TParaOperatorTable::AddOperator(TParaOperator* Operator, const TParaOperatorPriority& Priority)
{
    string Symbol = Operator->Symbol();

    // if the same operator symbol has already been registered,
    // it will be overwritten.
    if (_OperatorTable.count(Symbol) > 0) {
	delete _OperatorTable[Symbol];
    }
    
    int PriorityRank = Priority.PriorityRank();
    _PriorityTable[Symbol] = Priority;
    _OperatorTable[Symbol] = Operator;

    const string& BaseOperatorSymbol = Priority.BaseOperatorSymbol();
    if (! BaseOperatorSymbol.empty()) {
	if (_PriorityRankTable.count(BaseOperatorSymbol) > 0) {
	    int BasePriorityRank = _PriorityRankTable[BaseOperatorSymbol];
	    PriorityRank = BasePriorityRank + Priority.PriorityOffset();
	    _PriorityTable[Symbol].SetPriorityRank(PriorityRank);
	}
    }

    if (PriorityRank < 0) {
	PriorityRank = _LowestPriorityRank;
    }

    _PriorityRankTable[Symbol] = PriorityRank;
    if (_HighestPriorityRank > PriorityRank) {
        _HighestPriorityRank = PriorityRank;
    }
    if (_LowestPriorityRank < PriorityRank) {
        _LowestPriorityRank = PriorityRank;
    }
}

void TParaOperatorTable::AddPrepositionalOperator(TParaOperator* Operator)
{
    string Symbol = Operator->Symbol();

    // if the same operator symbol has already been registered,
    // it will be overwritten.
    if (_PrepositionalOperatorTable.count(Symbol) > 0) {
	delete _PrepositionalOperatorTable[Symbol];
    }

    _PrepositionalOperatorTable[Symbol] = Operator;
}

void TParaOperatorTable::AddPostpositionalOperator(TParaOperator* Operator)
{
    string Symbol = Operator->Symbol();

    // if the same operator symbol has already been registered,
    // it will be overwritten.
    if (_PostpositionalOperatorTable.count(Symbol) > 0) {
	delete _PostpositionalOperatorTable[Symbol];
    }

    _PostpositionalOperatorTable[Symbol] = Operator;
}

void TParaOperatorTable::AddElementaryOperator(TParaOperator* Operator)
{
    string Symbol = Operator->Symbol();

    // if the same operator symbol has already been registered,
    // it will be overwritten.
    if (_ElementaryOperatorTable.count(Symbol) > 0) {
	delete _ElementaryOperatorTable[Symbol];
    }

    _ElementaryOperatorTable[Symbol] = Operator;
}

TParaOperator* TParaOperatorTable::CreateOperator(const string& Symbol)
{
    if (_OperatorTable.count(Symbol) > 0) {
        return _OperatorTable[Symbol]->Clone();
    }
    else {
        return 0;
    }
}

TParaOperator* TParaOperatorTable::CreatePrepositionalOperator(const string& Symbol)
{
    if (_PrepositionalOperatorTable.count(Symbol) > 0) {
        return _PrepositionalOperatorTable[Symbol]->Clone();
    }
    else {
        return 0;
    }
}

TParaOperator* TParaOperatorTable::CreatePostpositionalOperator(const string& Symbol)
{
    if (_PostpositionalOperatorTable.count(Symbol) > 0) {
        return _PostpositionalOperatorTable[Symbol]->Clone();
    }
    else {
        return 0;
    }
}

TParaOperator* TParaOperatorTable::CreateElementaryOperator(const string& Symbol)
{
    if (_ElementaryOperatorTable.count(Symbol) > 0) {
        return _ElementaryOperatorTable[Symbol]->Clone();
    }
    else {
        return 0;
    }
}

const TParaOperatorPriority& TParaOperatorTable::PriorityOf(const string& Symbol)
{
    if (_OperatorTable.count(Symbol) == 0) {
        throw TScriptException(
	    "TParaOperatorTable::PriorityOf()",
	    "inconsistent operator table (internal error)"
	);
    }

    return _PriorityTable[Symbol];
}

int TParaOperatorTable::PriorityRankOf(const string& Symbol)
{
    if (_OperatorTable.count(Symbol) > 0) {
        return _PriorityRankTable[Symbol];
    }
    else {
        return 0;
    }
}

int TParaOperatorTable::HighestPriorityRank(void) const
{
    return _HighestPriorityRank;
}

int TParaOperatorTable::LowestPriorityRank(void) const
{
    return _LowestPriorityRank;
}



TParaCxxOperatorTable::TParaCxxOperatorTable(void)
{
    AddElementaryOperator(new TParaOperatorNew());
    AddElementaryOperator(new TParaOperatorListGenerate());
    AddElementaryOperator(new TParaOperatorVariableAccess());

    AddPrepositionalOperator(new TParaOperatorDelete());
    AddPrepositionalOperator(new TParaOperatorSizeOf());
    AddPrepositionalOperator(new TParaOperatorTypeOf());
    AddPrepositionalOperator(new TParaOperatorKeys());
    AddPrepositionalOperator(new TParaOperatorPointerReference());
    AddPrepositionalOperator(new TParaOperatorAddress());
    AddPrepositionalOperator(new TParaOperatorIncrement());
    AddPrepositionalOperator(new TParaOperatorDecrement());
    AddPrepositionalOperator(new TParaOperatorSignPlus());
    AddPrepositionalOperator(new TParaOperatorSignMinus());
    AddPrepositionalOperator(new TParaOperatorNot());
    AddPrepositionalOperator(new TParaOperatorBitReverse());

    //AddPostpositionalOperator(new TParaOperatorTypeCast());
    AddPostpositionalOperator(new TParaOperatorPostpositionalIncrement());
    AddPostpositionalOperator(new TParaOperatorPostpositionalDecrement());
    AddPostpositionalOperator(new TParaOperatorFactorial());
    AddPostpositionalOperator(new TParaOperatorPower());
    
    int PriorityRank;
    //AddOperator(new TParaOperatorPower(), PriorityRank = 2);
    AddOperator(new TParaOperatorMultiple(), PriorityRank = 4);
    AddOperator(new TParaOperatorDivide(), PriorityRank = 4);
    AddOperator(new TParaOperatorModulo(), PriorityRank = 4);
    AddOperator(new TParaOperatorAdd(), PriorityRank = 6);
    AddOperator(new TParaOperatorSubtract(), PriorityRank = 6);
    AddOperator(new TParaOperatorConcatenate(), PriorityRank = 6);
    AddOperator(new TParaOperatorLeftShift(), PriorityRank = 8);
    AddOperator(new TParaOperatorRightShift(), PriorityRank = 8);
    AddOperator(new TParaOperatorGreaterThan(), PriorityRank = 10);
    AddOperator(new TParaOperatorLessThan(), PriorityRank = 10);
    AddOperator(new TParaOperatorGreaterEqual(), PriorityRank = 10);
    AddOperator(new TParaOperatorLessEqual(), PriorityRank = 10);
    AddOperator(new TParaOperatorEqual(), PriorityRank = 12);
    AddOperator(new TParaOperatorNotEqual(), PriorityRank = 12);
    AddOperator(new TParaOperatorBitAnd(), PriorityRank = 14);
    AddOperator(new TParaOperatorBitXor(), PriorityRank = 16);
    AddOperator(new TParaOperatorBitOr(), PriorityRank = 18);
    AddOperator(new TParaOperatorListAnd(), PriorityRank = 14);
    AddOperator(new TParaOperatorAnd(), PriorityRank = 20);
    AddOperator(new TParaOperatorOr(), PriorityRank = 22);
    AddOperator(new TParaOperatorAssign(), PriorityRank = 24);
    AddOperator(new TParaOperatorAssignSum(), PriorityRank = 24);
    AddOperator(new TParaOperatorAssignDifference(), PriorityRank = 24);
    AddOperator(new TParaOperatorAssignProduct(), PriorityRank = 24);
    AddOperator(new TParaOperatorAssignQuotient(), PriorityRank = 24);
    AddOperator(new TParaOperatorAssignRemainder(), PriorityRank = 24);
    AddOperator(new TParaOperatorAssignConcatenation(), PriorityRank = 24);
}

TParaCxxOperatorTable::~TParaCxxOperatorTable()
{
}



TParaOperator::TParaOperator(void)
{
}

TParaOperator::~TParaOperator()
{
}

bool TParaOperator::IsLeftAssociative() const
{
    return true;
}

void TParaOperator::Parse(TParaTokenizer* Tokenizer, TParaExpressionParser* ExpressionParser, TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    Tokenizer->Next();
}

TParaValue& TParaOperator::EvaluateList(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException)
{
    if (Left.IsList() && Right.IsList()) {
	if (Left.AsValueList().size() != Right.AsValueList().size()) {
	    throw TScriptException("inconsistent list length");
	}
    }

    int Length = Left.IsList() ? Left.AsValueList().size() : Right.AsValueList().size();

    Result = TParaValue(TParaListValue(Length));
    vector<TParaValue>& ListResult = Result.AsValueList();
    ListResult.reserve(Length);

    for (int i = 0; i < Length; i++) {
	TParaValue& ThisLeft = Left.IsList() ? Left.AsValueList()[i] : Left;
	TParaValue& ThisRight = Right.IsList() ? Right.AsValueList()[i] : Right;
	TParaValue ElementResult;
	Evaluate(ThisLeft, ThisRight, SymbolTable, ElementResult);

	ListResult.push_back(ElementResult);
    }

    TParaListValue* InputListValue = 0;
    if (Left.IsList() && Left.AsConstList().HasKeyIndex()) {
	InputListValue = &Left.AsList();
    }
    else if (Right.IsList() && Right.AsConstList().HasKeyIndex()) {
	InputListValue = &Right.AsList();
    }
    if (InputListValue) {
	TParaListValue& ResultListValue = Result.AsList();
	for (int Index = 0; Index < Length; Index++) {
	    ResultListValue.SetKey(Index, InputListValue->KeyOf(Index));
	}
    }

    return Result;
}



TParaOperatorNew::TParaOperatorNew(void)
{
    _LengthExpression = 0;
}

TParaOperatorNew::~TParaOperatorNew()
{
    delete _LengthExpression;

    for (unsigned i = 0; i < _ArgumentList.size(); i++) {
	delete _ArgumentList[i];
    }
}

TParaOperator* TParaOperatorNew::Clone(void) const
{
    return new TParaOperatorNew();
}

string TParaOperatorNew::Symbol(void) const
{
    return string("new");
}

string TParaOperatorNew::Name(void) const
{
    return string("New");
}

void TParaOperatorNew::Parse(TParaTokenizer* Tokenizer, TParaExpressionParser* ExpressionParser, TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    Tokenizer->Next();
    _TypeName = Tokenizer->Next().AsString();

    while (Tokenizer->LookAhead().Is("*")) {
	_TypeName = "pointer";
    }

    if (Tokenizer->LookAhead().Is("[")) {
	Tokenizer->Next().MustBe("[");
	_LengthExpression = ExpressionParser->Parse(Tokenizer, SymbolTable);
	Tokenizer->Next().MustBe("]");
    }

    if (Tokenizer->LookAhead().Is("(")) {
	_ArgumentList = ExpressionParser->ParseExpressionList(
	    Tokenizer, SymbolTable, "(", ")", ","
	);
    }
}

TParaValue& TParaOperatorNew::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    int Length = 0;
    if (_LengthExpression != 0) {
	Length = _LengthExpression->Evaluate(SymbolTable).AsLong();
	if (Length < 1) {
	    throw TScriptException("new: invalid array length");
	}
    }

    vector<TParaValue*> ArgumentValueList;
    if (_ArgumentList.size() > 0) {
 	for (unsigned i = 0; i < _ArgumentList.size(); i++) {
	    TParaValue& ArgumentValue = _ArgumentList[i]->Evaluate(SymbolTable);
	    ArgumentValueList.push_back(&ArgumentValue);
	}
    }

    TParaValue* Instance = SymbolTable->CreateObject(_TypeName, Length);
    if (Instance == 0) {
        throw TScriptException("new: unknown variable type: " + _TypeName);
    }

    try {
	for (int i = 0; i < ((Length > 0) ? Length : 1); i++) {
	    if (Instance[i].IsObject()) {
		Instance[i].SetName(Instance[i].AsString());
		Instance[i].AsObject()->Construct(_TypeName, ArgumentValueList);
	    }
	    else {
		if (ArgumentValueList.size() > 1) {
		    throw TScriptException("new: too many initial values");
		}
		else if (ArgumentValueList.size() > 0) {
		    Instance[i].Assign(*ArgumentValueList[0]);
		}
	    }
	    Instance[i].SetLeftValueFlag();
	}
    }
    catch (TScriptException &e) {
	if (Length > 1) {
	    delete[] Instance;
	}
	else {
	    delete Instance;
	}
	throw;
    }

    return Result = TParaValue(Instance);
}



TParaOperatorDelete::TParaOperatorDelete(void)
{
    _IsForArray = false;
}

TParaOperatorDelete::~TParaOperatorDelete()
{
}

void TParaOperatorDelete::Parse(TParaTokenizer* Tokenizer, TParaExpressionParser* ExpressionParser, TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    Tokenizer->Next();
    
    if (Tokenizer->LookAhead().Is("[")) {
	Tokenizer->Next().MustBe("[");
	Tokenizer->Next().MustBe("]");
	_IsForArray = true;
    }
}

TParaOperator* TParaOperatorDelete::Clone(void) const
{
    return new TParaOperatorDelete();
}

string TParaOperatorDelete::Symbol(void) const
{
    return string("delete");
}

string TParaOperatorDelete::Name(void) const
{
    return string("Delete");
}

TParaValue& TParaOperatorDelete::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (Right.AsPointer() != 0) {
	TParaValue* Instance = Right.AsPointer();

	int Length = (_IsForArray) ? Instance->ArrayLength() : 0;
	for (int i = 0; i < ((Length > 0) ? Length : 1); i++) {
	    Instance[i].Destroy();
	}

	(Length > 0) ? delete[] Instance : delete Instance;
    }

    Right.Assign(TParaValue((TParaValue*) 0));
    
    return Result = TParaValue((TParaValue*) 0);
}



TParaSymbolTable* TParaOperatorVariableAccess::_MySymbolTable = 0;

TParaOperatorVariableAccess::TParaOperatorVariableAccess(void)
{
    _VariableNameExpression = 0;

    if (_MySymbolTable == 0) {
	_MySymbolTable = new TParaSymbolTable();
    }
}

TParaOperatorVariableAccess::~TParaOperatorVariableAccess()
{
    delete _VariableNameExpression;
#if 0
    delete _MySymbolTable;
    _MySymbolTable = 0;
#endif
}

TParaOperator* TParaOperatorVariableAccess::Clone(void) const
{
    return new TParaOperatorVariableAccess();
}

string TParaOperatorVariableAccess::Symbol(void) const
{
    return string("$");
}

string TParaOperatorVariableAccess::Name(void) const
{
    return string("VariableAccess");
}

void TParaOperatorVariableAccess::Parse(TParaTokenizer* Tokenizer, TParaExpressionParser* ExpressionParser, TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    Tokenizer->Next();
    if (Tokenizer->LookAhead().Is("{")) {
	Tokenizer->Next().MustBe("{");
	_VariableNameExpression = ExpressionParser->Parse(Tokenizer, SymbolTable);
	Tokenizer->Next().MustBe("}");
    }
    else {
	_VariableName = Tokenizer->Next().AsString();
	_VariableId = _MySymbolTable->NameToId(_VariableName);
    }
}

TParaValue& TParaOperatorVariableAccess::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException)
{
    if (_VariableNameExpression != 0) {
	_VariableName = _VariableNameExpression->Evaluate(SymbolTable).AsString();
	_VariableId = _MySymbolTable->NameToId(_VariableName);
    }

    TParaValue* Variable = _MySymbolTable->GetVariable(_VariableId);

    if (Variable == 0) {
	Variable = _MySymbolTable->CreateObject("variant");
	Variable->SetName(_VariableName);
	_MySymbolTable->RegisterVariable(_VariableId, Variable);
    }

    return *Variable;
}



TParaOperatorListGenerate::TParaOperatorListGenerate(void)
{
    _StartValueExpression = 0;
    _EndValueExpression = 0;
    _StepValueExpression = 0;
}

TParaOperatorListGenerate::~TParaOperatorListGenerate()
{
    delete _StartValueExpression;
    delete _EndValueExpression;
    delete _StepValueExpression;
}

TParaOperator* TParaOperatorListGenerate::Clone(void) const
{
    return new TParaOperatorListGenerate();
}

string TParaOperatorListGenerate::Symbol(void) const
{
    return string("[");
}

string TParaOperatorListGenerate::Name(void) const
{
    return string("ListGenerate");
}

void TParaOperatorListGenerate::Parse(TParaTokenizer* Tokenizer, TParaExpressionParser* ExpressionParser, TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    Tokenizer->Next().MustBe("[");

    _StartValueExpression = ExpressionParser->Parse(Tokenizer, SymbolTable);

    TParaToken SeparatorToken = Tokenizer->Next();
    _Separator = SeparatorToken.AsString();
    if ((_Separator != ",") && (_Separator != ":")) {
	SeparatorToken.ThrowUnexpected();
    }

    _EndValueExpression = ExpressionParser->Parse(Tokenizer, SymbolTable);

    if (Tokenizer->LookAhead().IsNot("]")) {
	Tokenizer->Next().MustBe(_Separator);
	_StepValueExpression = ExpressionParser->Parse(Tokenizer, SymbolTable);
	if (_Separator == ":") {
	    swap(_StepValueExpression, _EndValueExpression);
	}
    }

    Tokenizer->Next().MustBe("]");
}

TParaValue& TParaOperatorListGenerate::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException)
{
    TParaValue StartValue = _StartValueExpression->Evaluate(SymbolTable);
    TParaValue EndValue = _EndValueExpression->Evaluate(SymbolTable);

    TParaValue StepValue;
    if (_StepValueExpression != 0) {
	StepValue = _StepValueExpression->Evaluate(SymbolTable);
    }
    else {
	if (StartValue.AsDouble() < EndValue.AsDouble()) {
	    StepValue = TParaValue((long) 1);
	}
	else {
	    StepValue = TParaValue((long) -1);
	}
    }

    bool IsIntegerList = (
	StartValue.IsLong() && EndValue.IsLong() && StepValue.IsLong()
    );
    
    long LStart, LEnd, LStep;
    double DStart, DEnd, DStep;
    int NumberOfElements, StepSign;
    if (IsIntegerList) {
	LStart = StartValue.AsLong();
	LEnd = EndValue.AsLong();
	LStep = StepValue.AsLong();
	StepSign = (LStep > 0) ? 1 : -1;
	NumberOfElements = ((LEnd - LStart) / LStep) + 1;
    }    
    else {
	DStart = StartValue.AsDouble();
	DEnd = EndValue.AsDouble();
	DStep = StepValue.AsDouble();
	StepSign = (DStep > 0) ? 1 : -1;
	NumberOfElements = (int) ((DEnd - DStart) / DStep + 1.0e-6) + 1;
    }

    if (NumberOfElements < 0) {
	NumberOfElements = 0;
    }

    if (_Separator == ",") {
	if (IsIntegerList) {
	    if (LStart + (NumberOfElements-1) * LStep == LEnd) {
		NumberOfElements -= 1;
	    }
	}
	else {
	    double Last = DStart + (NumberOfElements-1) * DStep;
	    if (fabs((Last - DEnd) / DStep) < 1.0e-6) {
		NumberOfElements -= 1;
	    }
	}
    }

    Result = TParaValue(TParaListValue(NumberOfElements));
    vector<TParaValue>& ValueList = Result.AsValueList();
    
    for (int i = 0; i < NumberOfElements; i++) {
	if (IsIntegerList) {
	    ValueList.push_back(TParaValue(LStart + i * LStep));
	}
	else {
	    ValueList.push_back(TParaValue(DStart + i * DStep));
	}
    }

    return Result;
}



TParaOperator* TParaOperatorSizeOf::Clone(void) const
{
    return new TParaOperatorSizeOf();
}

string TParaOperatorSizeOf::Symbol(void) const
{
    return string("sizeof");
}

string TParaOperatorSizeOf::Name(void) const
{
    return string("SizeOf");
}

TParaValue& TParaOperatorSizeOf::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (Right.IsPointer()) {
	long Length = Right.AsPointer()->ArrayLength();
	Result = TParaValue((Length > 0) ? Length : (long) 1);
    }
    else if (Right.IsList()) {
	Result = TParaValue((long) Right.AsValueList().size());
    }
    else if (Right.IsString()) {
	Result = TParaValue((long) Right.AsString().size());
    }
    else {
	throw TScriptException("sizeof: array, list or string is expected");
    }

    return Result;
}



TParaOperator* TParaOperatorTypeOf::Clone(void) const
{
    return new TParaOperatorTypeOf();
}

string TParaOperatorTypeOf::Symbol(void) const
{
    return string("typeof");
}

string TParaOperatorTypeOf::Name(void) const
{
    return string("TypeOf");
}

TParaValue& TParaOperatorTypeOf::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    return Result = TParaValue(Right.TypeName());
}



TParaOperator* TParaOperatorKeys::Clone(void) const
{
    return new TParaOperatorKeys();
}

string TParaOperatorKeys::Symbol(void) const
{
    return string("keys");
}

string TParaOperatorKeys::Name(void) const
{
    return string("Keys");
}

TParaValue& TParaOperatorKeys::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (! Right.IsList()) {
	throw TScriptException("keys: list value is expected");
    }
    TParaListValue& ListValue = Right.AsList();

    Result = TParaValue(TParaListValue(ListValue.ListSize()));
    vector<TParaValue>& ResultList = Result.AsValueList();

    for (unsigned Index = 0; Index < ListValue.ListSize(); Index++) {
	ResultList.push_back(TParaValue(ListValue.KeyOf(Index)));
    }
    
    return Result;
}



TParaOperator* TParaOperatorPointerReference::Clone(void) const
{
    return new TParaOperatorPointerReference();
}

string TParaOperatorPointerReference::Symbol(void) const
{
    return string("*");
}

string TParaOperatorPointerReference::Name(void) const
{
    return string("PointerReference");
}

TParaValue& TParaOperatorPointerReference::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (! Right.IsPointer() /* && ! Right.IsLong() */) {
        throw TScriptException("pointer value is expected");
    }
    if (Right.AsLong() == 0) {
        throw TScriptException("null pointer reference");
    }
    
    return *(Right.AsPointer());
} 



TParaOperator* TParaOperatorAddress::Clone(void) const
{
    return new TParaOperatorAddress();
}

string TParaOperatorAddress::Symbol(void) const
{
    return string("&");
}

string TParaOperatorAddress::Name(void) const
{
    return string("Address");
}

TParaValue& TParaOperatorAddress::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (! Right.IsLeftValue()) {
        throw TScriptException("l-value is expected");
    }

    return Result = TParaValue(&Right);
} 



TParaOperator* TParaOperatorIncrement::Clone(void) const
{
    return new TParaOperatorIncrement();
}

string TParaOperatorIncrement::Symbol(void) const
{
    return string("++");
}

string TParaOperatorIncrement::Name(void) const
{
    return string("Increment");
}

TParaValue& TParaOperatorIncrement::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException)
{
    TParaValue Step((long) 1);
    TParaOperatorAdd().Evaluate(Right, Step, SymbolTable, Result);
    Right.Assign(Result);

    return Result;
}



TParaOperator* TParaOperatorDecrement::Clone(void) const
{
    return new TParaOperatorDecrement();
}

string TParaOperatorDecrement::Symbol(void) const
{
    return string("--");
}

string TParaOperatorDecrement::Name(void) const
{
    return string("Decrement");
}

TParaValue& TParaOperatorDecrement::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException)
{
    TParaValue Step((long) 1);
    TParaOperatorSubtract().Evaluate(Right, Step, SymbolTable, Result);
    Right.Assign(Result);

    return Result;
}



TParaOperator* TParaOperatorPostpositionalIncrement::Clone(void) const
{
    return new TParaOperatorPostpositionalIncrement();
}

string TParaOperatorPostpositionalIncrement::Symbol(void) const
{
    return string("++");
}

string TParaOperatorPostpositionalIncrement::Name(void) const
{
    return string("PostpositionalIncrement");
}

TParaValue& TParaOperatorPostpositionalIncrement::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException)
{
    Result = Left;

    TParaValue Step((long) 1);
    TParaValue NewValue;
    TParaOperatorAdd().Evaluate(Left, Step, SymbolTable, NewValue);
    Left.Assign(NewValue);

    return Result;
}



TParaOperator* TParaOperatorPostpositionalDecrement::Clone(void) const
{
    return new TParaOperatorPostpositionalDecrement();
}

string TParaOperatorPostpositionalDecrement::Symbol(void) const
{
    return string("--");
}

string TParaOperatorPostpositionalDecrement::Name(void) const
{
    return string("PostpositionalDecrement");
}

TParaValue& TParaOperatorPostpositionalDecrement::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException)
{
    Result = Left;

    TParaValue Step((long) 1);
    TParaValue NewValue;
    TParaOperatorSubtract().Evaluate(Left, Step, SymbolTable, NewValue);
    Left.Assign(NewValue);

    return Result;
}



TParaOperator* TParaOperatorSignPlus::Clone(void) const
{
    return new TParaOperatorSignPlus();
}

string TParaOperatorSignPlus::Symbol(void) const
{
    return string("+");
}

string TParaOperatorSignPlus::Name(void) const
{
    return string("SignPlus");
}

TParaValue& TParaOperatorSignPlus::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    Result = Right;
    return Result;
} 



TParaOperator* TParaOperatorSignMinus::Clone(void) const
{
    return new TParaOperatorSignMinus();
}

string TParaOperatorSignMinus::Symbol(void) const
{
    return string("-");
}

string TParaOperatorSignMinus::Name(void) const
{
    return string("SignMinus");
}

TParaValue& TParaOperatorSignMinus::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    TParaValue ZeroValue((long) 0);
    TParaOperatorSubtract().Evaluate(ZeroValue, Right, SymbolTable, Result);

    return Result;
}



TParaOperator* TParaOperatorNot::Clone(void) const
{
    return new TParaOperatorNot();
}

string TParaOperatorNot::Symbol(void) const
{
    return string("!");
}

string TParaOperatorNot::Name(void) const
{
    return string("Not");
}

TParaValue& TParaOperatorNot::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (Right.IsList()) {
	EvaluateList(Left, Right, SymbolTable, Result);
	return Result;
    }

    return Result = TParaValue(! Right.AsBool());
} 



TParaOperator* TParaOperatorBitReverse::Clone(void) const
{
    return new TParaOperatorBitReverse();
}

string TParaOperatorBitReverse::Symbol(void) const
{
    return string("~");
}

string TParaOperatorBitReverse::Name(void) const
{
    return string("BitReverse");
}

TParaValue& TParaOperatorBitReverse::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (Right.IsList()) {
	EvaluateList(Left, Right, SymbolTable, Result);
	return Result;
    }

    return Result = TParaValue((long) ~(Right.AsLong()));
} 



TParaOperator* TParaOperatorMultiple::Clone(void) const
{
    return new TParaOperatorMultiple();
}

string TParaOperatorMultiple::Symbol(void) const
{
    return string("*");
}

string TParaOperatorMultiple::Name(void) const
{
    return string("Multiple");
}

TParaValue& TParaOperatorMultiple::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (Left.IsList() || Right.IsList()) {
	EvaluateList(Left, Right, SymbolTable, Result);
	return Result;
    }

    if (! Left.IsNumeric() || ! Right.IsNumeric()) {
        throw TScriptException("integer or floating value is expected");
    }

    else if (Left.IsDouble() || Right.IsDouble()) {
        Result = TParaValue(Left.AsDouble() * Right.AsDouble());
    }
    else {
        Result = TParaValue(Left.AsLong() * Right.AsLong());
    }

    return Result;
} 



TParaOperator* TParaOperatorDivide::Clone(void) const
{
    return new TParaOperatorDivide();
}

string TParaOperatorDivide::Symbol(void) const
{
    return string("/");
}

string TParaOperatorDivide::Name(void) const
{
    return string("Divide");
}

TParaValue& TParaOperatorDivide::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (Left.IsList() || Right.IsList()) {
	EvaluateList(Left, Right, SymbolTable, Result);
	return Result;
    }

    if (! Left.IsNumeric() || ! Right.IsNumeric()) {
        throw TScriptException("integer or floating value is expected");
    }

    else if (Left.IsDouble() || Right.IsDouble()) {
	if (Right.AsDouble() == 0) {
	    throw TScriptException("divide by zero");
	}
        Result = TParaValue(Left.AsDouble() / Right.AsDouble());
    }
    else {
	if (Right.AsLong() == 0) {
	    throw TScriptException("divide by zero");
	}
        Result = TParaValue(Left.AsLong() / Right.AsLong());
    }

    return Result;
} 



TParaOperator* TParaOperatorModulo::Clone(void) const
{
    return new TParaOperatorModulo();
}

string TParaOperatorModulo::Symbol(void) const
{
    return string("%");
}

string TParaOperatorModulo::Name(void) const
{
    return string("Modulo");
}

TParaValue& TParaOperatorModulo::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (Left.IsList() || Right.IsList()) {
	EvaluateList(Left, Right, SymbolTable, Result);
	return Result;
    }

    if ((! Left.IsLong()) || (! Right.IsLong())) {
        throw TScriptException("integer value is expected");
    }

    else if (Right.AsLong() == 0) {
        throw TScriptException("divide by zero");
    }

    else {
	Result = TParaValue((long) Left.AsLong() % Right.AsLong());
    }

    return Result;
} 



TParaOperator* TParaOperatorAdd::Clone(void) const
{
    return new TParaOperatorAdd();
}

string TParaOperatorAdd::Symbol(void) const
{
    return string("+");
}

string TParaOperatorAdd::Name(void) const
{
    return string("Add");
}

TParaValue& TParaOperatorAdd::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException)
{
    if (Left.IsVoid()) {
        return Result = TParaValue(Right);
    }
    else if (Right.IsVoid()) {
        return Result = TParaValue(Left);
    }

    if (Left.IsList() || Right.IsList()) {
	EvaluateList(Left, Right, SymbolTable, Result);
	return Result;
    }

    if (Left.IsPointer() && Right.IsLong()) {
        Result = TParaValue(Left.AsPointer() + Right.AsLong());
    }
    else if (Right.IsPointer() && Left.IsLong()) {
        Result = TParaValue(Right.AsPointer() + Left.AsLong());
    }

    else if (Left.IsString() || Right.IsString()) {
        Result = TParaValue(Left.AsString() + Right.AsString());
    }

    else if (! Left.IsNumeric() || ! Right.IsNumeric()) {
        throw TScriptException("integer or floating value is expected");
    }

    else if (Left.IsDouble() || Right.IsDouble()) {
        Result = TParaValue(Left.AsDouble() + Right.AsDouble());
    }
    else {
        Result = TParaValue(Left.AsLong() + Right.AsLong());
    }

    return Result;
} 



TParaOperator* TParaOperatorSubtract::Clone(void) const
{
    return new TParaOperatorSubtract();
}

string TParaOperatorSubtract::Symbol(void) const
{
    return string("-");
}

string TParaOperatorSubtract::Name(void) const
{
    return string("Subtract");
}

TParaValue& TParaOperatorSubtract::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (Left.IsList() || Right.IsList()) {
	EvaluateList(Left, Right, SymbolTable, Result);
	return Result;
    }

    if (Left.IsPointer() && Right.IsLong()) {
        Result = TParaValue(Left.AsPointer() - Right.AsLong());
    }

    else if (Left.IsPointer() && Right.IsPointer()) {
        Result = TParaValue((long) (Left.AsPointer() - Right.AsPointer()));
    }

    else if (! Left.IsNumeric() || ! Right.IsNumeric()) {
        throw TScriptException("integer or floating value is expected");
    }

    else if (Left.IsDouble() || Right.IsDouble()) {
        Result = TParaValue(Left.AsDouble() - Right.AsDouble());
    }
    else {
        Result = TParaValue(Left.AsLong() - Right.AsLong());
    }

    return Result;
} 



TParaOperator* TParaOperatorConcatenate::Clone(void) const
{
    return new TParaOperatorConcatenate();
}

string TParaOperatorConcatenate::Symbol(void) const
{
    return string("<+>");
}

string TParaOperatorConcatenate::Name(void) const
{
    return string("Concatenate");
}

TParaValue& TParaOperatorConcatenate::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException)
{
    Result = TParaValue(TParaListValue());
    TParaListValue& ListValue = Result.AsList();

    if (Left.IsList()) {
	ListValue.AppendList(Left.AsList());
    }
    else {
	ListValue.AppendValue(Left);
    }
    
    if (Right.IsList()) {
	ListValue.AppendList(Right.AsList());
    }
    else {
	ListValue.AppendValue(Right);
    }

    return Result;
} 



TParaOperator* TParaOperatorLeftShift::Clone(void) const
{
    return new TParaOperatorLeftShift();
}

string TParaOperatorLeftShift::Symbol(void) const
{
    return string("<<");
}

string TParaOperatorLeftShift::Name(void) const
{
    return string("LeftShift");
}

TParaValue& TParaOperatorLeftShift::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (Left.IsList() || Right.IsList()) {
	EvaluateList(Left, Right, SymbolTable, Result);
	return Result;
    }

    return Result = TParaValue((long) (Left.AsLong() << Right.AsLong()));
} 



TParaOperator* TParaOperatorRightShift::Clone(void) const
{
    return new TParaOperatorRightShift();
}

string TParaOperatorRightShift::Symbol(void) const
{
    return string(">>");
}

string TParaOperatorRightShift::Name(void) const
{
    return string("RightShift");
}

TParaValue& TParaOperatorRightShift::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (Left.IsList() || Right.IsList()) {
	EvaluateList(Left, Right, SymbolTable, Result);
	return Result;
    }

    return Result = TParaValue((long) (Left.AsLong() >> Right.AsLong()));
} 



TParaOperator* TParaOperatorGreaterThan::Clone(void) const
{
    return new TParaOperatorGreaterThan();
}

string TParaOperatorGreaterThan::Symbol(void) const
{
    return string(">");
}

string TParaOperatorGreaterThan::Name(void) const
{
    return string("GreaterThan");
}

TParaValue& TParaOperatorGreaterThan::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (Left.IsList() || Right.IsList()) {
	EvaluateList(Left, Right, SymbolTable, Result);
	return Result;
    }

    return Result = TParaValue(Left.AsDouble() > Right.AsDouble());
} 



TParaOperator* TParaOperatorLessThan::Clone(void) const
{
    return new TParaOperatorLessThan();
}

string TParaOperatorLessThan::Symbol(void) const
{
    return string("<");
}

string TParaOperatorLessThan::Name(void) const
{
    return string("LessThan");
}

TParaValue& TParaOperatorLessThan::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (Left.IsList() || Right.IsList()) {
	EvaluateList(Left, Right, SymbolTable, Result);
	return Result;
    }

    return Result = TParaValue(Left.AsDouble() < Right.AsDouble());
} 



TParaOperator* TParaOperatorGreaterEqual::Clone(void) const
{
    return new TParaOperatorGreaterEqual();
}

string TParaOperatorGreaterEqual::Symbol(void) const
{
    return string(">=");
}

string TParaOperatorGreaterEqual::Name(void) const
{
    return string("GreaterEqual");
}

TParaValue& TParaOperatorGreaterEqual::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (Left.IsList() || Right.IsList()) {
	EvaluateList(Left, Right, SymbolTable, Result);
	return Result;
    }

    return Result = TParaValue(Left.AsDouble() >= Right.AsDouble());
} 



TParaOperator* TParaOperatorLessEqual::Clone(void) const
{
    return new TParaOperatorLessEqual();
}

string TParaOperatorLessEqual::Symbol(void) const
{
    return string("<=");
}

string TParaOperatorLessEqual::Name(void) const
{
    return string("LessEqual");
}

TParaValue& TParaOperatorLessEqual::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (Left.IsList() || Right.IsList()) {
	EvaluateList(Left, Right, SymbolTable, Result);
	return Result;
    }

    return Result = TParaValue(Left.AsDouble() <= Right.AsDouble());
} 



TParaOperator* TParaOperatorEqual::Clone(void) const
{
    return new TParaOperatorEqual();
}

string TParaOperatorEqual::Symbol(void) const
{
    return string("==");
}

string TParaOperatorEqual::Name(void) const
{
    return string("Equal");
}

TParaValue& TParaOperatorEqual::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (Left.IsList() || Right.IsList()) {
	EvaluateList(Left, Right, SymbolTable, Result);
	return Result;
    }

    bool Value;
    if (Left.IsPointer() || Right.IsPointer()) {
        Value = (Left.AsPointer() == Right.AsPointer());
    }

    else if (Left.IsObject() || Right.IsObject()) {
        Value = (((void*) Left.AsObject()) == ((void*) Right.AsObject()));
    }

    else if (Left.IsString() || Right.IsString()) {
        Value = (Left.AsString() == Right.AsString());
    }

    else if (Left.IsDouble() || Right.IsDouble()) {
        Value = (Left.AsDouble() == Right.AsDouble());
    }

    else {
        Value = (Left.AsLong() == Right.AsLong());
    }

    return Result = TParaValue(Value);
}



TParaOperator* TParaOperatorNotEqual::Clone(void) const
{
    return new TParaOperatorNotEqual();
}

string TParaOperatorNotEqual::Symbol(void) const
{
    return string("!=");
}

string TParaOperatorNotEqual::Name(void) const
{
    return string("NotEqual");
}

TParaValue& TParaOperatorNotEqual::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    TParaOperatorEqual().Evaluate(Left, Right, SymbolTable, Result);

    return Result = TParaValue(! Result.AsBool());
} 



TParaOperator* TParaOperatorBitAnd::Clone(void) const
{
    return new TParaOperatorBitAnd();
}

string TParaOperatorBitAnd::Symbol(void) const
{
    return string("&");
}

string TParaOperatorBitAnd::Name(void) const
{
    return string("BitAnd");
}

TParaValue& TParaOperatorBitAnd::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (Left.IsList() || Right.IsList()) {
	EvaluateList(Left, Right, SymbolTable, Result);
	return Result;
    }

    return Result = TParaValue((long) (Left.AsLong() & Right.AsLong()));
} 



TParaOperator* TParaOperatorBitXor::Clone(void) const
{
    return new TParaOperatorBitXor();
}

string TParaOperatorBitXor::Symbol(void) const
{
    return string("^");
}

string TParaOperatorBitXor::Name(void) const
{
    return string("BitXor");
}

TParaValue& TParaOperatorBitXor::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (Left.IsList() || Right.IsList()) {
	EvaluateList(Left, Right, SymbolTable, Result);
	return Result;
    }

    return Result = TParaValue((long) (Left.AsLong() ^ Right.AsLong()));
} 



TParaOperator* TParaOperatorBitOr::Clone(void) const
{
    return new TParaOperatorBitOr();
}

string TParaOperatorBitOr::Symbol(void) const
{
    return string("|");
}

string TParaOperatorBitOr::Name(void) const
{
    return string("BitOr");
}

TParaValue& TParaOperatorBitOr::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (Left.IsList() || Right.IsList()) {
	EvaluateList(Left, Right, SymbolTable, Result);
	return Result;
    }

    return Result = TParaValue((long) (Left.AsLong() | Right.AsLong()));
} 



TParaOperator* TParaOperatorListAnd::Clone(void) const
{
    return new TParaOperatorListAnd();
}

string TParaOperatorListAnd::Symbol(void) const
{
    return string("<&>");
}

string TParaOperatorListAnd::Name(void) const
{
    return string("ListAnd");
}

TParaValue& TParaOperatorListAnd::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    int LeftLength = (Left.IsList()) ? Left.AsValueList().size() : 1;
    int RightLength = (Right.IsList()) ? Right.AsValueList().size() : 1;

    Result = TParaValue(TParaListValue());
    vector<TParaValue>& ResultList = Result.AsValueList();

    TParaOperatorEqual OperatorEqual;
    TParaValue CompareResult;

    for (int i = 0; i < LeftLength; i++) {
	TParaValue& ThisLeft = (Left.IsList()) ? Left.AsValueList()[i] : Left;
	for (int j = 0; j < RightLength; j++) {
	    TParaValue& ThisRight = (Right.IsList()) ? Right.AsValueList()[j] : Right;
	    OperatorEqual.Evaluate(ThisLeft, ThisRight, SymbolTable, CompareResult);
	    if (CompareResult.AsBool()) {
		ResultList.push_back(ThisLeft);
	    }
	}
    }

    return Result;
} 



TParaOperator* TParaOperatorAnd::Clone(void) const
{
    return new TParaOperatorAnd();
}

string TParaOperatorAnd::Symbol(void) const
{
    return string("&&");
}

string TParaOperatorAnd::Name(void) const
{
    return string("And");
}

TParaValue& TParaOperatorAnd::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (Left.IsList() || Right.IsList()) {
	EvaluateList(Left, Right, SymbolTable, Result);
	return Result;
    }

    return Result = TParaValue(Left.AsBool() && Right.AsBool());
} 



TParaOperator* TParaOperatorOr::Clone(void) const
{
    return new TParaOperatorOr();
}

string TParaOperatorOr::Symbol(void) const
{
    return string("||");
}

string TParaOperatorOr::Name(void) const
{
    return string("Or");
}

TParaValue& TParaOperatorOr::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (Left.IsList() || Right.IsList()) {
	EvaluateList(Left, Right, SymbolTable, Result);
	return Result;
    }

    return Result = TParaValue(Left.AsBool() || Right.AsBool());
} 



TParaOperator* TParaOperatorAssign::Clone(void) const
{
    return new TParaOperatorAssign();
}

string TParaOperatorAssign::Symbol(void) const
{
    return string("=");
}

string TParaOperatorAssign::Name(void) const
{
    return string("Assign");
}

TParaValue& TParaOperatorAssign::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (! Left.IsLeftValue()) {
        throw TScriptException("l-value expected");
    }

    Left.Assign(Right);

    return Left;
}

bool TParaOperatorAssign::IsLeftAssociative() const
{
    return false;
}



TParaOperator* TParaOperatorAssignSum::Clone(void) const
{
    return new TParaOperatorAssignSum();
}

string TParaOperatorAssignSum::Symbol(void) const
{
    return string("+=");
}

string TParaOperatorAssignSum::Name(void) const
{
    return string("AssignSum");
}

TParaValue& TParaOperatorAssignSum::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (! Left.IsLeftValue()) {
        throw TScriptException("l-value expected");
    }

    TParaOperatorAdd().Evaluate(Left, Right, SymbolTable, Result);
    Left.Assign(Result);

    return Left;
} 

bool TParaOperatorAssignSum::IsLeftAssociative() const
{
    return false;
}



TParaOperator* TParaOperatorAssignDifference::Clone(void) const
{
    return new TParaOperatorAssignDifference();
}

string TParaOperatorAssignDifference::Symbol(void) const
{
    return string("-=");
}

string TParaOperatorAssignDifference::Name(void) const
{
    return string("AssignDifference");
}

TParaValue& TParaOperatorAssignDifference::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (! Left.IsLeftValue()) {
        throw TScriptException("l-value expected");
    }

    TParaOperatorSubtract().Evaluate(Left, Right, SymbolTable, Result);
    Left.Assign(Result);

    return Left;
} 

bool TParaOperatorAssignDifference::IsLeftAssociative() const
{
    return false;
}



TParaOperator* TParaOperatorAssignProduct::Clone(void) const
{
    return new TParaOperatorAssignProduct();
}

string TParaOperatorAssignProduct::Symbol(void) const
{
    return string("*=");
}

string TParaOperatorAssignProduct::Name(void) const
{
    return string("AssignProduct");
}

TParaValue& TParaOperatorAssignProduct::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (! Left.IsLeftValue()) {
        throw TScriptException("l-value expected");
    }

    TParaOperatorMultiple().Evaluate(Left, Right, SymbolTable, Result);
    Left.Assign(Result);

    return Left;
} 

bool TParaOperatorAssignProduct::IsLeftAssociative() const
{
    return false;
}



TParaOperator* TParaOperatorAssignQuotient::Clone(void) const
{
    return new TParaOperatorAssignQuotient();
}

string TParaOperatorAssignQuotient::Symbol(void) const
{
    return string("/=");
}

string TParaOperatorAssignQuotient::Name(void) const
{
    return string("AssignQuotient");
}

TParaValue& TParaOperatorAssignQuotient::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (! Left.IsLeftValue()) {
        throw TScriptException("l-value expected");
    }

    TParaOperatorDivide().Evaluate(Left, Right, SymbolTable, Result);
    Left.Assign(Result);

    return Left;
} 

bool TParaOperatorAssignQuotient::IsLeftAssociative() const
{
    return false;
}



TParaOperator* TParaOperatorAssignRemainder::Clone(void) const
{
    return new TParaOperatorAssignRemainder();
}

string TParaOperatorAssignRemainder::Symbol(void) const
{
    return string("%=");
}

string TParaOperatorAssignRemainder::Name(void) const
{
    return string("AssignRemainder");
}

TParaValue& TParaOperatorAssignRemainder::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (! Left.IsLeftValue()) {
        throw TScriptException("l-value expected");
    }

    TParaOperatorModulo().Evaluate(Left, Right, SymbolTable, Result);
    Left.Assign(Result);

    return Left;
} 

bool TParaOperatorAssignRemainder::IsLeftAssociative() const
{
    return false;
}



TParaOperator* TParaOperatorAssignConcatenation::Clone(void) const
{
    return new TParaOperatorAssignConcatenation();
}

string TParaOperatorAssignConcatenation::Symbol(void) const
{
    return string("<+>=");
}

string TParaOperatorAssignConcatenation::Name(void) const
{
    return string("AssignConcatenation");
}

TParaValue& TParaOperatorAssignConcatenation::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (! Left.IsLeftValue()) {
        throw TScriptException("l-value expected");
    }

    if (Left.IsList()) {
	if (Right.IsList()) {
	    Left.AsList().AppendList(Right.AsList());
	}
	else {
	    Left.AsList().AppendValue(Right);
	}
    }
    else {
	throw TScriptException("list value is expected");
    }

    return Left;
} 

bool TParaOperatorAssignConcatenation::IsLeftAssociative() const
{
    return false;
}



TParaOperator* TParaOperatorFactorial::Clone(void) const
{
    return new TParaOperatorFactorial();
}

string TParaOperatorFactorial::Symbol(void) const
{
    return string("!");
}

string TParaOperatorFactorial::Name(void) const
{
    return string("Factorial");
}

TParaValue& TParaOperatorFactorial::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
    if (Left.IsList() || Right.IsList()) {
	EvaluateList(Left, Right, SymbolTable, Result);
	return Result;
    }

    long Value = 1;
    for (long n = Left.AsLong(); n > 0; n--) {
	Value *= n;
    }

    return Result = TParaValue(Value);
} 




TParaOperatorPower::TParaOperatorPower(void)
{
    _PowerExpression = 0;
}

TParaOperatorPower::~TParaOperatorPower()
{
    delete _PowerExpression;
}

TParaOperator* TParaOperatorPower::Clone(void) const
{
    return new TParaOperatorPower();
}

string TParaOperatorPower::Symbol(void) const
{
    return string("**");
}

string TParaOperatorPower::Name(void) const
{
    return string("Power");
}

void TParaOperatorPower::Parse(TParaTokenizer* Tokenizer, TParaExpressionParser* ExpressionParser, TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    Tokenizer->Next().MustBe(Symbol());

#if 1
    // use the power operator as a postpositional unary operator,
    // to make -1**2 == -(1**2) = -1.
    _PowerExpression = ExpressionParser->ParseElement(Tokenizer, SymbolTable);
#else
    // use the power operator as a normal binary operator:
    // -1**2 becomes (-1)**2 = 1.
    _PowerExpression = 0;
#endif
}

TParaValue& TParaOperatorPower::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException) 
{
#if 1
    // use the power operator as a postpositional unary operator.
    TParaValue* Power;
    if (Right.IsVoid()) {
	// postpositional operator node sets the right operand to VOID
	Power = &_PowerExpression->Evaluate(SymbolTable);
    }
    else {
	// EvaluateList() calls this method recursively with RightValue.
	Power = &Right;
    }
#else
    // use the power operator as a normal binary operator.
    TParaValue* Power = &Right;
#endif

    if (Power->IsList() || Left.IsList()) {
	EvaluateList(Left, *Power, SymbolTable, Result);
	return Result;
    }

    if (! Left.IsNumeric() || ! Power->IsNumeric()) {
        throw TScriptException("integer or floating value is expected");
    }

    return Result = TParaValue((double) pow(Left.AsDouble(), Power->AsDouble()));
} 
