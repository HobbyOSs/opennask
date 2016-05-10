/* ParaSymbolTable.cc */
/* Created by Enomoto Sanshiro on 27 August 1999. */
/* Last updated by Enomoto Sanshiro on 2 September 2000. */


#include <string>
#include <vector>
#include "ParaValue.hh"
#include "ParaObject.hh"
#include "ParaFunction.hh"
#include "ParaBuiltinFunction.hh"
#include "ParaSymbolTable.hh"

using namespace std;


TParaNameTable* TParaNameTable::_Instance = 0;

TParaNameTable* TParaNameTable::GetInstance(void)
{
    if (_Instance == 0) {
        _Instance = new TParaNameTable();
    }

    return _Instance;
}

TParaNameTable::TParaNameTable(void)
{
    _NextId = 1;
}

TParaNameTable::~TParaNameTable()
{
}

long TParaNameTable::NameToId(const string& Name)
{
    if (_IdTable.count(Name) == 0) {
        _IdTable[Name] = _NextId;
	_NextId++;
    }
    
    return _IdTable[Name];
}

string TParaNameTable::IdToName(long Id)
{
    map<string, long>::iterator Entry;
    for (
        Entry = _IdTable.begin();
        Entry != _IdTable.end();
        Entry++
    ){
        if ((*Entry).second == Id) {
            return (*Entry).first;
        }
    }
    
    return string("");
}



TParaSymbolTable::TParaSymbolTable(TParaObjectPrototypeTable* ObjectPrototypeTable, TParaBuiltinFunctionTable* BuiltinFunctionTable)
{
    _NameTable = TParaNameTable::GetInstance();

    _ObjectPrototypeTable = ObjectPrototypeTable;
    _BuiltinFunctionTable = BuiltinFunctionTable;

    _OriginalFunctionTable = new map<long, TParaFunction*>;
    _FunctionTable = _OriginalFunctionTable;

    _VariableCount = 0;
    _CurrentBlockDepth = 0;
}
    
TParaSymbolTable::TParaSymbolTable(TParaSymbolTable* SymbolTable, int ImportDepth)
{
    _OriginalFunctionTable = 0;
    Import(SymbolTable, ImportDepth);

    _VariableCount = 0;
    _CurrentBlockDepth = 0;
}

TParaSymbolTable::~TParaSymbolTable()
{
    delete _OriginalFunctionTable;

    for (unsigned i = 0; i < _GlobalVariableList.size(); i++) {
        TParaValue* Variable = _GlobalVariableList[i];
	Variable->Destroy();
	delete Variable;
    }
}

bool TParaSymbolTable::IsTypeName(const string& Symbol) const
{
    if (
	(Symbol == "int") || (Symbol == "long") ||
	(Symbol == "float") || (Symbol == "double") ||
	(Symbol == "string")  || (Symbol == "list") ||
	(Symbol == "pointer") || 
	(Symbol == "variant") ||
	(Symbol == "bool")
    ){
        return true;
    }

    else if (_ObjectPrototypeTable != 0) {
        return _ObjectPrototypeTable->IsRegisteredClassName(Symbol);
    }
    
    return false;
}

TParaValue* TParaSymbolTable::CreateObject(const string& TypeName, int Length)
{
    TParaValue* Value;
    if (Length > 0) {
	Value = new TParaValue[Length];
    }
    else {
	Value = new TParaValue;
    }
    
    for (int i = 0; i < ((Length > 0) ? Length : 1); i++) {
	if (TypeName == "void") {
	    Value[i] = TParaValue();
	}
	else if (TypeName == "bool") {
	    Value[i] = TParaValue(false);
	}
	else if ((TypeName == "int") || (TypeName == "long")) {
	    Value[i] = TParaValue((long) 0);
	}
	else if ((TypeName == "float") || (TypeName == "double")) {
	    Value[i] = TParaValue((double) 0);
	}
	else if (TypeName == "string") {
	    Value[i] = TParaValue(string(""));
	}
	else if (TypeName == "list") {
	    Value[i] = TParaValue(TParaListValue());
	}
	else if ((TypeName == "pointer") || (TypeName == "*")) {
	    Value[i] = TParaValue((TParaValue*) 0);
	}
	else if (TypeName == "variant") {
	    TParaVariant Variant;
	    Value[i] = TParaValue(Variant);
	}
	else {
	    TParaObjectPrototype* Object;
	    Object = _ObjectPrototypeTable->CreateInstance(TypeName);
	    if (Object) {
		Value[i] = TParaValue(Object);
	    }
	    else {
		(Length > 0) ? delete[] Value : delete Value;
		return 0;
	    }
	}

	Value[i].SetArrayLength(Length - i);
    }

    return Value;
}

void TParaSymbolTable::EnterBlock(void)
{
    _VariableCountList.push_back(_VariableCount);

    _VariableCount = 0;
    _CurrentBlockDepth++;
}

void TParaSymbolTable::ExitBlock(void) throw(TScriptException)
{
    for (; _VariableCount > 0; _VariableCount--) {
        TParaValue* Variable = _VariableEntryList.back().second;

	Variable->Destroy();
	if (Variable->IsArray()) {
	    TParaValue* ElementArray = Variable->AsPointer();
	    delete[] ElementArray;
	}
	delete Variable;

        _VariableEntryList.pop_back();
    }

    _VariableCount = _VariableCountList.back();
    _VariableCountList.pop_back();
    _CurrentBlockDepth--;
}

long TParaSymbolTable::NameToId(const string& Name)
{
    return _NameTable->NameToId(Name);
}

string TParaSymbolTable::IdToName(long Id)
{
    return _NameTable->IdToName(Id);
}

long TParaSymbolTable::RegisterVariable(const std::string& Name, const TParaValue& InitialValue)
{
    long VariableId = NameToId(Name);

    TParaValue* Variable;
    if (InitialValue.IsObject()) {
	TParaObjectPrototype* Object = InitialValue.AsObject()->Clone();
	Variable = new TParaValue(Object);
    }
    else {
	Variable = new TParaValue(InitialValue);
    }
	
    return RegisterVariable(VariableId, Variable);
}

long TParaSymbolTable::RegisterVariable(const string& Name, TParaValue* Variable)
{
    long VariableId = NameToId(Name);
    return RegisterVariable(VariableId, Variable);
}

long TParaSymbolTable::RegisterVariable(long VariableId, TParaValue* Variable)
{
    Variable->SetLeftValueFlag();
    string VariableName = IdToName(VariableId);

    pair<long, TParaValue*> VariableEntry(VariableId, Variable);
    _VariableEntryList.push_back(VariableEntry);
    _VariableCount++;

    if (_CurrentBlockDepth == 0) {
	_GlobalVariableList.push_back(Variable);
    }

    return VariableId;
}

TParaValue* TParaSymbolTable::GetVariable(long VariableId)
{
    TParaValue* Variable = 0;

    vector<pair<long, TParaValue*> >::reverse_iterator VariableEntry;

    for (
	VariableEntry = _VariableEntryList.rbegin();
	VariableEntry != _VariableEntryList.rend();
	VariableEntry++
    ){
	if ((*VariableEntry).first == VariableId) {
	    Variable = (*VariableEntry).second;
	    break;
	}
    }

    return Variable;
}
    
long TParaSymbolTable::Import(TParaSymbolTable* SymbolTable, int Depth)
{
    long NumberOfImported = 0;
    vector<long>::const_iterator VariableCountIterator;
    vector<pair<long, TParaValue*> >::const_iterator VariableEntryIterator;

    VariableCountIterator = SymbolTable->_VariableCountList.begin();
    VariableEntryIterator = SymbolTable->_VariableEntryList.begin();
    
    for (int i = 0; i < Depth; i++) {
        long NumberOfVariables;
	if (VariableCountIterator != SymbolTable->_VariableCountList.end()) {
	    NumberOfVariables = *VariableCountIterator;
	}
	else {
	    NumberOfVariables = SymbolTable->_VariableCount;
	}

	for (int j = 0; j < NumberOfVariables; j++) {
            _VariableEntryList.push_back(*VariableEntryIterator);
            VariableEntryIterator++;
            NumberOfImported++;
        }
	
        _VariableCountList.push_back(NumberOfVariables);

	if (VariableCountIterator != SymbolTable->_VariableCountList.end()) {
	    VariableCountIterator++;
	}
	else {
	    break;
	}
    }
        
    _NameTable = SymbolTable->_NameTable;
    _ObjectPrototypeTable = SymbolTable->_ObjectPrototypeTable;
    _BuiltinFunctionTable = SymbolTable->_BuiltinFunctionTable;
    _FunctionTable = SymbolTable->_FunctionTable;

    return NumberOfImported;
}

void TParaSymbolTable::RegisterFunction(long FunctionId, TParaFunction* Function)
{
    (*_FunctionTable)[FunctionId] = Function;
}
                   
TParaFunction* TParaSymbolTable::GetFunction(long FunctionId)
{
    if (_FunctionTable->count(FunctionId) == 0) {
        return 0;
    }

    return (*_FunctionTable)[FunctionId];
}

TParaObjectPrototypeTable* TParaSymbolTable::ObjectPrototypeTable(void)
{
    return _ObjectPrototypeTable;
}

TParaBuiltinFunctionTable* TParaSymbolTable::BuiltinFunctionTable(void)
{
    return _BuiltinFunctionTable;
}
