/* ParaContainerLibrary.cc */
/* Created by Enomoto Sanshiro on 22 April 2000. */
/* Last updated by Enomoto Sanshiro on 22 April 2000. */


#include <string>
#include <vector>
#include "ParaObject.hh"
#include "ParaContainerLibrary.hh"

using namespace std;


// This class will be removed... do not use.

TParaVectorObject::TParaVectorObject(void)
: TParaObjectPrototype("Vector")
{
}

TParaVectorObject::~TParaVectorObject()
{
}

TParaObjectPrototype* TParaVectorObject::Clone(void)
{
    return new TParaVectorObject();
}

int TParaVectorObject::DispatchMessage(const string& Message, vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false)
{
    if (Message == "putAt") {
        ReturnValue = PutAt(ArgumentList);
    }
    else if (Message == "getAt") {
        ReturnValue = GetAt(ArgumentList);
    }
    else if (Message == "pushBack") {
        ReturnValue = PushBack(ArgumentList);
    }
    else if (Message == "size") {
        ReturnValue = Size(ArgumentList);
    }
    else {
	return 0;
    }

    return 1;
}

TParaValue TParaVectorObject::PutAt(vector<TParaValue*>& ArgumentList) noexcept(false)
{
    if (ArgumentList.size() < 2) {
        throw TScriptException(
	    "Too few arguments: Vector::putAt(int index, TParaValue value)"
	);
    }

    long Index = ArgumentList[0]->AsLong();
    TParaValue* Value = ArgumentList[1];

    if (Index >= (long) _VectorStorage.capacity()) {
	int NewSize = (int) (_VectorStorage.capacity() * 1.5);
	if (Index >= NewSize) {
	    NewSize = Index + 1;
	}
	_VectorStorage.resize(NewSize);
    }

    _VectorStorage[Index] = *Value;
    
    return *Value;
}

TParaValue TParaVectorObject::GetAt(vector<TParaValue*>& ArgumentList) noexcept(false)
{
    if (ArgumentList.size() < 1) {
        throw TScriptException(
	    "Too few arguments: Vector::getAt(int index)"
	);
    }

    long Index = ArgumentList[0]->AsLong();

    if (Index >= (long) _VectorStorage.size()) {
	throw TScriptException("Index overrun");
    }
    
    return _VectorStorage[Index];
}

TParaValue TParaVectorObject::PushBack(vector<TParaValue*>& ArgumentList) noexcept(false)
{
    if (ArgumentList.size() < 1) {
        throw TScriptException(
	    "Too few arguments: Vector::pushBack(TParaValue value)"
	);
    }

    _VectorStorage.push_back(*ArgumentList[0]);
    
    return *ArgumentList[0]; 
}

TParaValue TParaVectorObject::Size(vector<TParaValue*>& ArgumentList) noexcept(false)
{
    return TParaValue((long) _VectorStorage.size()); 
}

