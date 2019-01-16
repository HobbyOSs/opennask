/* ParaBuiltinFunction.cc */
/* Created by Enomoto Sanshiro on 27 August 2000. */
/* Last updated by Enomoto Sanshiro on 27 August 2000. */


#include <string>
#include <vector>
#include <map>
#include "ParaObject.hh"
#include "ParaValue.hh"
#include "ParaBuiltinFunction.hh"

using namespace std;


TParaBuiltinFunctionTable::TParaBuiltinFunctionTable(void)
{
}

TParaBuiltinFunctionTable::~TParaBuiltinFunctionTable(void)
{
    for (unsigned i = 0; i < _PrototypeList.size(); i++) {
	delete _PrototypeList[i];
    }
}

void TParaBuiltinFunctionTable::Merge(TParaBuiltinFunctionTable* Source)
{
    for (unsigned i = 0; i < Source->_PrototypeList.size(); i++) {
	RegisterAnonymousClass(Source->_PrototypeList[i]->Clone());
    }
}

void TParaBuiltinFunctionTable::RegisterAnonymousClass(TParaObjectPrototype* Prototype)
{
    _PrototypeList.push_back(Prototype);
}

void TParaBuiltinFunctionTable::RegisterFunctionId(const std::string& FunctionName, long FunctionId)
{
    if (_ClassIdTable.count(FunctionId) > 0) {
	return;
    }

    int ClassId, MethodId;
    for (ClassId = (int) _PrototypeList.size() - 1; ClassId >= 0; ClassId--) {
	MethodId = _PrototypeList[ClassId]->MethodIdOf(FunctionName);
	if (MethodId > 0) {
	    _ClassIdTable[FunctionId] = ClassId;
	    _MethodIdTable[FunctionId] = MethodId;
	    break;
	}
    }

    _MethodNameTable[FunctionId] = FunctionName;
}

int TParaBuiltinFunctionTable::Execute(long FunctionId, vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false)
{
    int Result = 0;

    map<long, int>::iterator ClassIdEntry = _ClassIdTable.find(FunctionId);
    if (ClassIdEntry != _ClassIdTable.end()) {
	long ClassId = (*ClassIdEntry).second;
	long MethodId = _MethodIdTable[FunctionId];

	Result = _PrototypeList[ClassId]->InvokeMethod(
	    MethodId, ArgumentList, ReturnValue
	);
    }

    if (Result == 0) {
	string MethodName = _MethodNameTable[FunctionId];
	for (int i = (int) _PrototypeList.size() - 1; i >= 0; i--) {
	    Result = _PrototypeList[i]->DispatchMessage(
		MethodName, ArgumentList, ReturnValue
	    );
	    if (Result != 0) {
		break;
	    }
	}
    }

    return Result;
}
