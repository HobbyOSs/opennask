/* ParaExpression.cc */
/* Created by Enomoto Sanshiro on 22 August 1999. */
/* Last updated by Enomoto Sanshiro on 3 July 2002. */


#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include "ParaObject.hh"
#include "ParaValue.hh"
#include "ParaOperator.hh"
#include "ParaSymbolTable.hh"
#include "ParaExpression.hh"
#include "ParaFunction.hh"
#include "ParaBuiltinFunction.hh"

using namespace std;


TParaExpressionParser::TParaExpressionParser(TParaOperatorTable* OperatorTable)
{
    _OperatorTable = OperatorTable;
}

TParaExpressionParser::~TParaExpressionParser()
{
}

TParaExpression* TParaExpressionParser::Parse(TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable, int PriorityRank) throw(TScriptException)
{
    TParaExpression* ThisNode = 0;

    if (PriorityRank < 0) {
        /* default priority: start from the lowest priority (highest level) */
        PriorityRank = _OperatorTable->LowestPriorityRank();
    }
    if (PriorityRank < _OperatorTable->HighestPriorityRank()) {
        return ParsePrepositional(Tokenizer, SymbolTable);
    }

    ThisNode = Parse(Tokenizer, SymbolTable, PriorityRank - 1);

    while (1) {
	string OperatorSymbol = Tokenizer->LookAhead().AsString();
	if (_OperatorTable->PriorityRankOf(OperatorSymbol) != PriorityRank) {
	    break;
	}

	TParaExpression* LeftNode = ThisNode;
	TParaExpression* RightNode = 0;
	TParaOperator* Operator = _OperatorTable->CreateOperator(OperatorSymbol);

	long LineNumber = Tokenizer->LineNumber();
	try {
	    Operator->Parse(Tokenizer, this, SymbolTable);
	}
	catch (TScriptException &e) {
	    delete Operator;
	    throw;
	}

	int RightNodePriorityRank = PriorityRank - 1;
	if (! Operator->IsLeftAssociative()) {
	    RightNodePriorityRank = PriorityRank;
	}

	try {
	    RightNode = Parse(Tokenizer, SymbolTable, RightNodePriorityRank);
	}
	catch (TScriptException &e) {
	    delete Operator;
	    delete LeftNode;
	    throw;
	}
	ThisNode = new TParaOperatorNode(Operator, LeftNode, RightNode);
	ThisNode->SetLineNumber(LineNumber);

	if (! Operator->IsLeftAssociative()) {
	    break;
	}
    }

    return ThisNode;
}

TParaExpression* TParaExpressionParser::ParsePrepositional(TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    TParaExpression* ThisNode = 0;
    long LineNumber = Tokenizer->LineNumber();

    TParaToken Token = Tokenizer->LookAhead();
    string OperatorSymbol = Token.AsString();
    TParaOperator* PrepositionalOperator = _OperatorTable->CreatePrepositionalOperator(OperatorSymbol);

    try {
	// normal prepositional operator //
	if (PrepositionalOperator != 0) {
	    PrepositionalOperator->Parse(Tokenizer, this, SymbolTable);
	    TParaExpression* RightNode = ParsePrepositional(Tokenizer, SymbolTable);
	    ThisNode = new TParaOperatorNode(PrepositionalOperator, 0, RightNode);
	    ThisNode->SetLineNumber(LineNumber);
	}

	// typecast //
	// (int), (Scanner), (int*), (int***) etc.
	// the third condition is to make distinction from (Scanner(line).get())
	else if (
	    Tokenizer->LookAhead(1).Is("(") &&
	    SymbolTable->IsTypeName(Tokenizer->LookAhead(2).AsString()) &&
	    Tokenizer->LookAhead(3).IsNot("(")
	){
	    ThisNode = ParseTypeCast(Tokenizer, SymbolTable);
	}

	// go lower: element //
	else {
	    ThisNode = ParseElement(Tokenizer, SymbolTable);
	}
    }
    catch (TScriptException &e) {
	delete PrepositionalOperator;
	throw;
    }

    return ThisNode;
}

TParaExpression* TParaExpressionParser::ParseElement(TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    TParaExpression* ThisNode = 0;
    TParaToken Token = Tokenizer->Next();
    long LineNumber = Tokenizer->LineNumber();

    string OperatorSymbol = Token.AsString();
    TParaOperator* ElementaryOperator = _OperatorTable->CreateElementaryOperator(OperatorSymbol);

    if (ElementaryOperator != 0) {
	Tokenizer->Unget(Token);
	ElementaryOperator->Parse(Tokenizer, this, SymbolTable);
	ThisNode = new TParaOperatorNode(ElementaryOperator, 0, 0);
	ThisNode->SetLineNumber(LineNumber);
    }

    else if (Token.Is("(")) {
	ThisNode = Parse(Tokenizer, SymbolTable);
	Tokenizer->Next().MustBe(")");
    }

    // primitive literals //
    else if (Token.IsInteger()) {
        ThisNode = new TParaLiteralNode(TParaValue(Token.AsLong()));
	ThisNode->SetLineNumber(LineNumber);
    }
    else if (Token.IsFloating()) {
        ThisNode = new TParaLiteralNode(TParaValue(Token.AsDouble()));
	ThisNode->SetLineNumber(LineNumber);
    }
    else if (Token.IsQuote()) {
        Token.RemoveQuotation();
        ThisNode = new TParaLiteralNode(TParaValue(Token.AsString()));
	ThisNode->SetLineNumber(LineNumber);
    }

    // temporary object creation //
    else if (SymbolTable->IsTypeName(Token.AsString())) {
	Tokenizer->Unget(Token);
	ThisNode = ParseTemporaryObjectCreation(Tokenizer, SymbolTable);
    }

    // identifier: function call or variable //
    else if (Token.IsIdentifier()) {
        if (Tokenizer->LookAhead().Is("(")) {
            Tokenizer->Unget(Token);
            ThisNode = ParseFunctionCall(Tokenizer, SymbolTable);
        }
        else {
            long VariableId = SymbolTable->NameToId(Token.AsString());
            ThisNode = new TParaVariableNode(VariableId);
	    ThisNode->SetLineNumber(LineNumber);
        }
    }

    // list literal //
    else if (Token.Is("{")) {
	Tokenizer->Unget(Token);
        ThisNode = ParseListExpression(Tokenizer, SymbolTable);
	ThisNode->SetLineNumber(LineNumber);
    }

    else {
	Token.ThrowUnexpected();
    }

    return ParsePostpositional(ThisNode, Tokenizer, SymbolTable);
}

TParaExpression* TParaExpressionParser::ParseTypeCast(TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    long LineNumber = Tokenizer->LineNumber();

    Tokenizer->Next().MustBe("(");
    string TypeName = Tokenizer->Next().AsString();
    while (Tokenizer->LookAhead().Is("*")) {
	TypeName = "pointer";
	Tokenizer->Next();
    }
    Tokenizer->Next().MustBe(")");

    TParaExpression* RightNode = ParsePrepositional(Tokenizer, SymbolTable);

    TParaExpression* ThisNode = new TParaTypeCastNode(TypeName, RightNode);
    ThisNode->SetLineNumber(LineNumber);

    return ThisNode;
}

TParaExpression* TParaExpressionParser::ParseTemporaryObjectCreation(TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    long LineNumber = Tokenizer->LineNumber();

    TParaToken Token = Tokenizer->Next();
    string TypeName = Token.AsString();

    if (! SymbolTable->IsTypeName(Token.AsString())) {
	Token.ThrowUnexpected("type name");
    }

    vector<TParaExpression*> ArgumentExpressionList;
    ArgumentExpressionList = ParseExpressionList(
	Tokenizer, SymbolTable, "(", ")", ","
    );

    TParaExpression* ThisNode = new TParaTemporaryObjectCreationNode(
	TypeName, ArgumentExpressionList
    );
    ThisNode->SetLineNumber(LineNumber);

    return ThisNode;
}

TParaFunctionCallNode* TParaExpressionParser::ParseFunctionCall(TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    long LineNumber = Tokenizer->LineNumber();

    TParaToken Token = Tokenizer->Next();
    if (! Token.IsIdentifier()) {
	Token.ThrowUnexpected("function name");
    }
    long FunctionId = SymbolTable->NameToId(Token.AsString());

    vector<TParaExpression*> ArgumentExpressionList;
    ArgumentExpressionList = ParseExpressionList(
	Tokenizer, SymbolTable, "(", ")", ","
    );

    TParaFunctionCallNode* ThisNode = new TParaFunctionCallNode(
	FunctionId, ArgumentExpressionList
    );
    ThisNode->SetLineNumber(LineNumber);

    return ThisNode;
}

TParaExpression* TParaExpressionParser::ParseListExpression(TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    vector<TParaExpression*> KeyExpressionList;
    vector<TParaExpression*> ValueExpressionList;

    TParaToken Token;
    Tokenizer->Next().MustBe("{");

    if ((Token = Tokenizer->Next()).IsNot("}")) {
        Tokenizer->Unget(Token);

	TParaExpression* Expression;
        while (1) {
	    Expression = Parse(Tokenizer, SymbolTable);

            Token = Tokenizer->Next();
	    if (Token.Is("=>")) {
		KeyExpressionList.push_back(Expression);
		Expression = Parse(Tokenizer, SymbolTable);
		Token = Tokenizer->Next();
	    }
	    else {
		KeyExpressionList.push_back((TParaExpression*) 0);
	    }

	    ValueExpressionList.push_back(Expression);

            if (Token.Is("}")) {
                break;
            }
            else {
                Token.MustBe(",");
            }
        }
    }

    return new TParaListNode(KeyExpressionList, ValueExpressionList);
}

TParaExpression* TParaExpressionParser::ParsePostpositional(TParaExpression* Expression, TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    TParaExpression* ThisNode = Expression;
    TParaOperator* Operator;

    TParaToken Token = Tokenizer->LookAhead();
    Operator = _OperatorTable->CreatePostpositionalOperator(Token.AsString());

    if (Operator != 0) {
	Operator->Parse(Tokenizer, this, SymbolTable);
	ThisNode = new TParaOperatorNode(Operator, ThisNode, 0);
	ThisNode->SetLineNumber(Tokenizer->LineNumber());
    }
    else if (Token.Is(".") || Token.Is("->")) {
        ThisNode = ParseMethodInvocation(ThisNode, Tokenizer, SymbolTable);
    }
    else if (Token.Is("[")) {
        ThisNode = ParseArrayReference(ThisNode, Tokenizer, SymbolTable);
    }
    else if (Token.Is("{")) {
        ThisNode = ParseTableReference(ThisNode, Tokenizer, SymbolTable);
    }
    else {
	return ThisNode;
    }

    return ParsePostpositional(ThisNode, Tokenizer, SymbolTable);
}

TParaExpression* TParaExpressionParser::ParseMethodInvocation(TParaExpression* ObjectNode, TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    long LineNumber = Tokenizer->LineNumber();

    if (Tokenizer->Next().Is("->")) {
        TParaOperator* Operator = new TParaOperatorPointerReference();
        ObjectNode = new TParaOperatorNode(Operator, 0, ObjectNode);
	ObjectNode->SetLineNumber(LineNumber);
    }

    TParaToken Token = Tokenizer->Next();
    if (! Token.IsIdentifier()) {
	Token.ThrowUnexpected("method or property name");
    }
    string MethodName = Token.AsString();
    long MethodId = SymbolTable->NameToId(MethodName);

    TParaExpression* ThisNode = 0;
    if (Tokenizer->LookAhead().IsNot("(")) {
	ThisNode = new TParaPropertyAccessNode(ObjectNode, MethodName);
    }
    else {
	vector<TParaExpression*> ArgumentExpressionList;
	ArgumentExpressionList = ParseExpressionList(
	    Tokenizer, SymbolTable, "(", ")", ","
	);

	ThisNode = new TParaMethodInvocationNode(
	    ObjectNode, MethodId, ArgumentExpressionList
	);
    }

    ThisNode->SetLineNumber(LineNumber);

    return ThisNode;
}

TParaExpression* TParaExpressionParser::ParseArrayReference(TParaExpression* ObjectNode, TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    long LineNumber = Tokenizer->LineNumber();

    Tokenizer->Next().MustBe("[");
    TParaExpression* IndexExpression = Parse(Tokenizer, SymbolTable);
    Tokenizer->Next().MustBe("]");

    TParaExpression* ThisNode = new TParaArrayReferenceNode(
        ObjectNode, IndexExpression
    );

    ThisNode->SetLineNumber(LineNumber);

    return ThisNode;
}

TParaExpression* TParaExpressionParser::ParseTableReference(TParaExpression* ObjectNode, TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    long LineNumber = Tokenizer->LineNumber();

    Tokenizer->Next().MustBe("{");
    TParaExpression* ThisNode = new TParaTableReferenceNode(
        ObjectNode, Parse(Tokenizer, SymbolTable)
    );

    try {
	Tokenizer->Next().MustBe("}");
    }
    catch (TScriptException &e) {
	delete ThisNode;
	throw;
    }

    ThisNode->SetLineNumber(LineNumber);

    return ThisNode;
}

vector<TParaExpression*> TParaExpressionParser::ParseExpressionList(TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable, const string& StartToken, const string& StopToken, const string& SeparatorToken) throw(TScriptException)
{
    vector<TParaExpression*> ArgumentExpressionList;

    TParaToken Token;
    Tokenizer->Next().MustBe(StartToken);

    if ((Token = Tokenizer->Next()).IsNot(StopToken)) {
        Tokenizer->Unget(Token);

        while (1) {
            TParaExpression* Argument = Parse(Tokenizer, SymbolTable);
            ArgumentExpressionList.push_back(Argument);

            Token = Tokenizer->Next();
            if (Token.Is(StopToken)) {
                break;
            }
            else {
                Token.MustBe(SeparatorToken);
            }
        }
    }

    return ArgumentExpressionList;
}



TParaExpression::TParaExpression()
{
    _LeftNode = 0;
    _RightNode = 0;
}

TParaExpression::~TParaExpression()
{
    delete _LeftNode;
    delete _RightNode;
}

void TParaExpression::SetLineNumber(long LineNumber)
{
    _LineNumber = LineNumber;
}

string TParaExpression::Position(void) const
{
    if (_LineNumber == 0) {
	return string("");
    }

    ostringstream Stream;
    Stream << "line " << _LineNumber << ": ";

    return Stream.str();
}

void TParaExpression::Dump(ostream &os, int IndentLevel) const
{
    if (_LeftNode != 0) {
        _LeftNode->Dump(os, IndentLevel + 1);
    }

    for (int i = 0; i < IndentLevel; i++) {
        os << "  ";
    }
    DumpThis(os);
    os << endl;

    if (_RightNode != 0) {
        _RightNode->Dump(os, IndentLevel + 1);
    }
}



TParaOperatorNode::TParaOperatorNode(TParaOperator* Operator, TParaExpression* LeftNode, TParaExpression* RightNode)
{
    _Operator = Operator;
    _LeftNode = LeftNode ? LeftNode : new TParaLiteralNode(TParaValue());
    _RightNode = RightNode ? RightNode : new TParaLiteralNode(TParaValue());
}

TParaOperatorNode::~TParaOperatorNode()
{
    delete _Operator;
}

TParaValue& TParaOperatorNode::Evaluate(TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    TParaValue& LeftValue = _LeftNode->Evaluate(SymbolTable);
    TParaValue& RightValue = _RightNode->Evaluate(SymbolTable);

    try {
	//...
	if (LeftValue.IsVariant() && (_Operator->Symbol() == "=")) {
	    return _Operator->Evaluate(
		LeftValue, RightValue, SymbolTable, _Value
	    );
	}

	if (LeftValue.IsObject()) {
	    return LeftValue.AsObject()->EvaluateOperator(
		_Operator, LeftValue, RightValue, SymbolTable, _Value
	    );
	}
	else if (RightValue.IsObject()) {
	    return RightValue.AsObject()->EvaluateOperator(
		_Operator, LeftValue, RightValue, SymbolTable, _Value
	    );
	}
	else {
	    return _Operator->Evaluate(
		LeftValue, RightValue, SymbolTable, _Value
	    );
	}
    }
    catch (TScriptException &e) {
	throw TScriptException(Position() + e.Message());
    }
}

void TParaOperatorNode::DumpThis(ostream &os) const
{
    os << _Operator->Symbol();
}



TParaTypeCastNode::TParaTypeCastNode(const string& TypeName, TParaExpression* RightNode)
{
    _TypeName = TypeName;

    _LeftNode = 0;
    _RightNode = RightNode;
}

TParaTypeCastNode::~TParaTypeCastNode()
{
}

TParaValue& TParaTypeCastNode::Evaluate(TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    TParaValue& Right = _RightNode->Evaluate(SymbolTable);

    try {
	if (_TypeName == "void") {
	    _Value = TParaValue();
	}
	else if (_TypeName == "bool") {
	    _Value = TParaValue(Right.AsBool());
	}
	else if ((_TypeName == "int") || (_TypeName == "long")) {
	    _Value = TParaValue(Right.AsLong());
	}
	else if ((_TypeName == "float") || (_TypeName == "double")) {
	    _Value = TParaValue(Right.AsDouble());
	}
	else if (_TypeName == "string") {
	    _Value = TParaValue(Right.AsString());
	}
	else if ((_TypeName == "pointer") || (_TypeName == "*")) {
	    _Value = TParaValue(Right.AsPointer());
	}
	else{
	    throw TScriptException("invalid type name");
	}
    }
    catch (TScriptException &e) {
	throw TScriptException(
	    "invalid type cast: " + _TypeName + ": " + e.Message()
	);
    }

    return _Value;
}

void TParaTypeCastNode::DumpThis(ostream &os) const
{
    os << "(" << _TypeName << ")";
}



TParaLiteralNode::TParaLiteralNode(const TParaValue& Value)
: _Value(Value)
{
}

TParaLiteralNode::~TParaLiteralNode()
{
}

TParaValue& TParaLiteralNode::Evaluate(TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    return _Value;
}

void TParaLiteralNode::DumpThis(ostream &os) const
{
    os << _Value.AsString();
}



TParaVariableNode::TParaVariableNode(long VariableId)
{
    _VariableId = VariableId;
}

TParaVariableNode::~TParaVariableNode()
{
}

TParaValue& TParaVariableNode::Evaluate(TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    TParaValue* Variable = SymbolTable->GetVariable(_VariableId);
    if (Variable == 0) {
        string Name = SymbolTable->IdToName(_VariableId);
        throw TScriptException(Position() + "undefined variable: " + Name);
    }

    return *Variable;
}

void TParaVariableNode::DumpThis(ostream &os) const
{
    os << TParaNameTable::GetInstance()->IdToName(_VariableId);
    os << "{" << _VariableId << "}";
}



TParaListNode::TParaListNode(const vector<TParaExpression*>& KeyExpressionList, const vector<TParaExpression*>& ValueExpressionList)
{
    _KeyExpressionList = KeyExpressionList;
    _ValueExpressionList = ValueExpressionList;
}

TParaListNode::~TParaListNode()
{
    for (unsigned i = 0; i < _KeyExpressionList.size(); i++) {
	delete _KeyExpressionList[i];
	delete _ValueExpressionList[i];
    }
}

TParaValue& TParaListNode::Evaluate(TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    _Value = TParaValue(TParaListValue());
    TParaListValue& ListValue = _Value.AsList();
    vector<TParaValue>& ValueList = _Value.AsValueList();

    for (unsigned i = 0; i < _ValueExpressionList.size(); i++) {
	TParaValue& Value = _ValueExpressionList[i]->Evaluate(SymbolTable);
	if (_KeyExpressionList[i] != 0) {
	    TParaValue& Key = _KeyExpressionList[i]->Evaluate(SymbolTable);
	    ListValue.ValueOf(Key.AsString()) = Value;
	}
	else {
	    ValueList.push_back(Value);
	}
    }

    return _Value;
}

void TParaListNode::DumpThis(ostream &os) const
{
    os << "list[" << _ValueExpressionList.size() << "]";
}



TParaFunctionCallNode::TParaFunctionCallNode(long FunctionId, vector<TParaExpression*>& ArgumentExpressionList)
: _Value((long) 0)
{
    _FunctionId = FunctionId;
    _ArgumentExpressionList = ArgumentExpressionList;

    _BuiltinFunctionTable = 0;
}

TParaFunctionCallNode::~TParaFunctionCallNode()
{
    for (unsigned i = 0; i < _ArgumentExpressionList.size(); i++) {
	delete _ArgumentExpressionList[i];
    }
}

TParaValue& TParaFunctionCallNode::Evaluate(TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    // EvaluateArgument() and ExecuteFunction() are separated
    // for delayed execution

    EvaluateArguments(SymbolTable);
    TParaValue& Result = ExecuteFunction(SymbolTable);

    return Result;
}

void TParaFunctionCallNode::EvaluateArguments(TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    _ArgumentList.erase(
        _ArgumentList.begin(), _ArgumentList.end()
    );

    vector<TParaExpression*>::iterator Expression;
    for (
        Expression = _ArgumentExpressionList.begin();
        Expression != _ArgumentExpressionList.end();
        Expression++
    ){
        TParaValue& Value = (*Expression)->Evaluate(SymbolTable);
        _ArgumentList.push_back(&Value);
    }
}

TParaValue& TParaFunctionCallNode::ExecuteFunction(TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    int Result = 0;

    TParaFunction* Function = SymbolTable->GetFunction(_FunctionId);
    if (Function != 0) {
	long ImportDepth;
	TParaSymbolTable* GlobalSymbolTable;
	GlobalSymbolTable = new TParaSymbolTable(SymbolTable, ImportDepth = 1);
	try {
	    _Value = Function->Execute(_ArgumentList, GlobalSymbolTable);
	}
	catch (TScriptException &e) {
	    delete GlobalSymbolTable;
	    throw;
	}
	delete GlobalSymbolTable;
	Result = 1;
    }
    else {
	if (_BuiltinFunctionTable == 0) {
	    _BuiltinFunctionTable = SymbolTable->BuiltinFunctionTable();
	    if (_BuiltinFunctionTable != 0) {
		_BuiltinFunctionTable->RegisterFunctionId(
		    SymbolTable->IdToName(_FunctionId), _FunctionId
		);
	    }
	}

	if (_BuiltinFunctionTable != 0) {
	    try {
		Result = _BuiltinFunctionTable->Execute(
		    _FunctionId, _ArgumentList, _Value
		);
	    }
	    catch (TScriptException &e) {
		throw TScriptException(Position() + e.Message());
	    }
	}
    }

    if (Result == 0) {
	string FunctionName = SymbolTable->IdToName(_FunctionId);
	throw TScriptException(
	    Position() + "unknown function: " + FunctionName + "()"
	);
    }

    return _Value;
}

void TParaFunctionCallNode::DumpThis(ostream &os) const
{
    string FunctionName = TParaNameTable::GetInstance()->IdToName(_FunctionId);
    os << FunctionName << "(...)";
}



TParaMethodInvocationNode::TParaMethodInvocationNode(TParaExpression* ObjectExpression, long FunctionId, vector<TParaExpression*>& ArgumentExpressionList)
: TParaFunctionCallNode(FunctionId, ArgumentExpressionList)
{
    _ObjectExpression = ObjectExpression;
    _MethodId = -1;
}

TParaMethodInvocationNode::~TParaMethodInvocationNode()
{
    delete _ObjectExpression;
}

TParaValue& TParaMethodInvocationNode::Evaluate(TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    TParaValue& ObjectValue = _ObjectExpression->Evaluate(SymbolTable);
    TParaObjectPrototype* Object;
    try {
	Object = ObjectValue.AsObject();
    }
    catch (TScriptException &e) {
	throw TScriptException(Position() + e.Message());
    }

    EvaluateArguments(SymbolTable);

    if (_MethodId < 0) {
	//... BUG: 'Object' can be a instance of a different class
        // from one in the previous call (when _ObjectExpression is
        // a pointer reference, for example).
	// _MethodId becomes wrong in such cases.

	_MethodName = SymbolTable->IdToName(_FunctionId);
	_MethodId = Object->MethodIdOf(_MethodName);
    }

    try {
	int Result = 0;
	if (_MethodId > 0) {
	    Result = Object->InvokeMethod(_MethodId, _ArgumentList, _Value);
	}
	if (Result == 0) {
	    Result = Object->DispatchMessage(_MethodName, _ArgumentList, _Value);
	}
	if (Result == 0) {
	    throw TScriptException("unknown method: " + _MethodName);
	}
    }
    catch (TScriptException &e) {
	throw TScriptException(Position() + e.Message());
    }

    return _Value;
}



TParaPropertyAccessNode::TParaPropertyAccessNode(TParaExpression* ObjectExpression, const string& PropertyName)
{
    _ObjectExpression = ObjectExpression;
    _PropertyName = PropertyName;
}

TParaPropertyAccessNode::~TParaPropertyAccessNode()
{
    delete _ObjectExpression;
}

TParaValue& TParaPropertyAccessNode::Evaluate(TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    TParaValue& ObjectValue = _ObjectExpression->Evaluate(SymbolTable);
    TParaObjectPrototype* Object;
    try {
	Object = ObjectValue.AsObject();
    }
    catch (TScriptException &e) {
	throw TScriptException(Position() + e.Message());
    }

    try {
	//... TODO: Object->GetProperty(PropertyId, _Value)
	int Result = Object->GetPropertyOf(_PropertyName, _Value);
	if (Result == 0) {
	    throw TScriptException("unknown property: " + _PropertyName);
	}
    }
    catch (TScriptException &e) {
	throw TScriptException(Position() + e.Message());
    }

    return _Value;
}

void TParaPropertyAccessNode::DumpThis(ostream &os) const
{
    os << "." << _PropertyName;
}



TParaTemporaryObjectCreationNode::TParaTemporaryObjectCreationNode(const string& TypeName, vector<TParaExpression*>& ArgumentExpressionList)
: TParaFunctionCallNode(0, ArgumentExpressionList)
{
    _TypeName = TypeName;
}

TParaTemporaryObjectCreationNode::~TParaTemporaryObjectCreationNode()
{
}

TParaValue& TParaTemporaryObjectCreationNode::Evaluate(TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    EvaluateArguments(SymbolTable);

    TParaValue* Variable = SymbolTable->CreateObject(_TypeName);
    if (Variable == 0) {
        throw TScriptException(
	    Position() + "unknown variable type: " + _TypeName
	);
    }
    long VariableId = -1;  // unused value for temporary objects
    SymbolTable->RegisterVariable(VariableId, Variable);

    if (Variable->IsObject()) {
	Variable->AsObject()->Construct(_TypeName, _ArgumentList);
    }
    else {
        if (_ArgumentList.size() > 1) {
	    throw TScriptException("too many initial-value arguments");
	}
	else if (! _ArgumentList.empty()) {
	    Variable->Assign(*_ArgumentList[0]);
	}
    }

    return *Variable;
}



TParaArrayReferenceNode::TParaArrayReferenceNode(TParaExpression* ObjectExpression, TParaExpression* IndexExpression)
{
    _ObjectExpression = ObjectExpression;
    _IndexExpression = IndexExpression;
}

TParaArrayReferenceNode::~TParaArrayReferenceNode()
{
    delete _IndexExpression;
    delete _ObjectExpression;
}

TParaValue& TParaArrayReferenceNode::Evaluate(TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    TParaValue& ObjectValue = _ObjectExpression->Evaluate(SymbolTable);
    TParaValue& IndexValue = _IndexExpression->Evaluate(SymbolTable);

    try {
	if (ObjectValue.IsList() && IndexValue.IsList()) {
	    return PartialListOf(ObjectValue, IndexValue);
	}
    }
    catch (TScriptException &e) {
	throw TScriptException(Position() + "operator[]: " + e.Message());
    }

    long Index;
    try {
	Index = IndexValue.AsLong();
    }
    catch (TScriptException &e) {
	throw TScriptException(Position() + "operator[]: " + e.Message());
    }

    if (ObjectValue.IsList()) {
	return ListItemOf(ObjectValue, Index);
    }

    if (ObjectValue.IsString()) {
	return StringItemOf(ObjectValue, Index);
    }

    if (ObjectValue.IsPointer()) {
	if ((Index < 0) || (Index >= ObjectValue.AsPointer()->ArrayLength())) {
	    throw TScriptException(
		Position() + "operator[]: index out of range"
	    );
	}
	return ObjectValue.AsPointer()[Index];
    }

    throw TScriptException(
	Position() + "operator[]: list, array or string is expected"
    );
}

TParaValue& TParaArrayReferenceNode::ListItemOf(TParaValue& ListValue, int Index) throw(TScriptException)
{
    vector<TParaValue>& List = ListValue.AsValueList();
    int ListSize = List.size();

    if (Index < 0) {
	Index = ListSize - ((abs(Index) - 1) % ListSize + 1);
    }

    if (Index >= ListSize) {
	TParaVariant Variant;
	List.resize(Index + 1, TParaValue(Variant));
	if (ListValue.IsLeftValue()) {
	    ListValue.SetLeftValueFlag();
	}
    }

    return List[Index];
}

TParaValue& TParaArrayReferenceNode::PartialListOf(TParaValue& ObjectListValue, TParaValue& IndexListValue) throw(TScriptException)
{
    _Value = TParaValue(TParaListValue());

    vector<TParaValue>& ObjectList = ObjectListValue.AsValueList();
    vector<TParaValue>& IndexList = IndexListValue.AsValueList();
    vector<TParaValue>& ResultList = _Value.AsValueList();

    for (unsigned i = 0; i < IndexList.size(); i++) {
	long Index = IndexList[i].AsLong();
	if ((Index >= 0) && (Index < (long) ObjectList.size())) {
	    ResultList.push_back(ObjectList[Index]);
	}
    }

    return _Value;
}

TParaValue& TParaArrayReferenceNode::StringItemOf(TParaValue& StringValue, int Index) throw(TScriptException)
{
    string& String = StringValue.AsStringReference();
    int StringSize = (int) String.size();

    if (Index < 0) {
	Index = StringSize - ((abs(Index) - 1) % StringSize + 1);
    }

    if (Index >= StringSize) {
	_Value = TParaValue(string());
    }
    else {
	_Value = TParaValue(String.substr(Index, 1));
    }

    return _Value;
}

void TParaArrayReferenceNode::DumpThis(std::ostream &os) const
{
    os << "[]";
}



TParaTableReferenceNode::TParaTableReferenceNode(TParaExpression* ObjectExpression, TParaExpression* IndexExpression)
{
    _ObjectExpression = ObjectExpression;
    _IndexExpression = IndexExpression;
}

TParaTableReferenceNode::~TParaTableReferenceNode()
{
    delete _ObjectExpression;
    delete _IndexExpression;
}

TParaValue& TParaTableReferenceNode::Evaluate(TParaSymbolTable* SymbolTable) throw(TScriptException)
{
    TParaValue& ObjectValue = _ObjectExpression->Evaluate(SymbolTable);
    TParaValue& IndexValue = _IndexExpression->Evaluate(SymbolTable);

    if (! ObjectValue.IsList()) {
	throw TScriptException(
	    Position() + "operator{}: list value is expected"
	);
    }

    try {
	if (IndexValue.IsList()) {
	    return PartialListOf(ObjectValue, IndexValue);
	}
	else {
	    string Key = _IndexExpression->Evaluate(SymbolTable).AsString();
	    return ObjectValue.AsList().ValueOf(Key);
	}
    }
    catch (TScriptException &e) {
	throw TScriptException(Position() + "operator{}: " + e.Message());
    }
}

TParaValue& TParaTableReferenceNode::PartialListOf(TParaValue& ObjectListValue, TParaValue& IndexListValue) throw(TScriptException)
{
    _Value = TParaValue(TParaListValue());

    vector<TParaValue>& IndexList = IndexListValue.AsValueList();
    TParaListValue& ObjectList = ObjectListValue.AsList();
    TParaListValue& ResultList = _Value.AsList();

    for (unsigned i = 0; i < IndexList.size(); i++) {
	string Key = IndexList[i].AsString();
	int Index = ResultList.AppendValue(ObjectList.ValueOf(Key));
	ResultList.SetKey(Index, Key);
    }

    return _Value;
}

void TParaTableReferenceNode::DumpThis(std::ostream &os) const
{
    os << "{}";
}
