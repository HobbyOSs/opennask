/* ParaObject.cc */
/* Created by Enomoto Sanshiro on 22 August 1999. */
/* Last updated by Enomoto Sanshiro on 3 July 2002. */


#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include "ParaDefs.hh"
#include "ParaObject.hh"
#include "ParaOperator.hh"
#include "ParaSymbolTable.hh"

using namespace std;


TParaObjectPrototypeTable::TParaObjectPrototypeTable(void)
{
}

TParaObjectPrototypeTable::~TParaObjectPrototypeTable()
{
    map<string, TParaObjectPrototype*>::iterator PrototypeIterator;
    for (
        PrototypeIterator = _PrototypeTable.begin();
        PrototypeIterator != _PrototypeTable.end();
        PrototypeIterator++
    ){
        TParaObjectPrototype* Prototype = (*PrototypeIterator).second;
        delete Prototype;
    }
}

void TParaObjectPrototypeTable::Merge(TParaObjectPrototypeTable* Source)
{
    map<string, TParaObjectPrototype*>& SourceTable = Source->_PrototypeTable;
    map<string, TParaObjectPrototype*>::iterator PrototypeIterator;
    for (
        PrototypeIterator = SourceTable.begin();
        PrototypeIterator != SourceTable.end();
        PrototypeIterator++
    ){
	string ClassName = (*PrototypeIterator).first;
        TParaObjectPrototype* Prototype = (*PrototypeIterator).second;
	RegisterClass(ClassName, Prototype->Clone());
    }
}

void TParaObjectPrototypeTable::RegisterClass(const string& ClassName, TParaObjectPrototype* ObjectPrototype)
{
    if (_PrototypeTable.count(ClassName) > 0) {
	delete _PrototypeTable[ClassName];
    }
	
    _PrototypeTable[ClassName] = ObjectPrototype;
}

void TParaObjectPrototypeTable::RegisterClass(TParaObjectPrototype* ObjectPrototype)
{
    string ClassName = ObjectPrototype->InternalClassName();
    RegisterClass(ClassName, ObjectPrototype);
}

TParaObjectPrototype* TParaObjectPrototypeTable::CreateInstance(const string& ClassName)
{
    if (_PrototypeTable.count(ClassName) == 0) {
        return 0;
    }
    
    TParaObjectPrototype* Prototype = _PrototypeTable[ClassName];
    TParaObjectPrototype* Instance = Prototype->Clone();

    return Instance;
}

TParaObjectPrototype* TParaObjectPrototypeTable::ReferenceClass(const string& ClassName)
{
    if (_PrototypeTable.count(ClassName) == 0) {
        return 0;
    }
    
    return _PrototypeTable[ClassName];
}

bool TParaObjectPrototypeTable::IsRegisteredClassName(const string& Name) const
{
    if (_PrototypeTable.count(Name) == 0) {
        return false;
    }
    else {
        return true;
    }
}



TParaObjectPrototype::TParaObjectPrototype(const string& InternalClassName)
: _InternalClassName(InternalClassName)
{
    _ObjectName = "NoName";
}

TParaObjectPrototype::~TParaObjectPrototype()
{
}

string TParaObjectPrototype::InternalClassName(void) const
{
    return _InternalClassName;
}

string TParaObjectPrototype::ObjectName(void) const
{
    return _ObjectName;
}

void TParaObjectPrototype::SetObjectName(const string& ObjectName)
{
    _ObjectName = ObjectName;
}

void TParaObjectPrototype::Construct(const string& ClassName, vector<TParaValue*>& ArgumentList) throw(TScriptException)
{
}

void TParaObjectPrototype::Destruct(void) throw(TScriptException)
{
}

int TParaObjectPrototype::MethodIdOf(const std::string& MethodName)
{
    return MethodId_Undefined;
}

int TParaObjectPrototype::InvokeMethod(int MethodId, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    return 0;    
}

int TParaObjectPrototype::DispatchMessage(const std::string& Message, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    return 0;
}

int TParaObjectPrototype::GetPropertyOf(const std::string& PropertyName, TParaValue& ReturnValue) throw(TScriptException)
{
    return 0;
}

TParaValue& TParaObjectPrototype::EvaluateOperator(TParaOperator* Operator, TParaValue& LeftValue, TParaValue& RightValue, TParaSymbolTable* SymbolTable, TParaValue& Result) throw(TScriptException)
{
    return Operator->Evaluate(LeftValue, RightValue, SymbolTable, Result);
}
