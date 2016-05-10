/* ParaBuiltinFunction.hh */
/* Created by Enomoto Sanshiro on 27 August 2000. */
/* Last updated by Enomoto Sanshiro on 27 August 2000. */


#ifndef __ParaBuiltinFunction_hh__
#define __ParaBuiltinFunction_hh__


#include <string>
#include <vector>
#include <map>
#include "ParaObject.hh"
#include "ParaValue.hh"
#include "ParaSymbolTable.hh"


class TParaBuiltinFunctionTable {
  public:
    TParaBuiltinFunctionTable(void);
    virtual ~TParaBuiltinFunctionTable(void);
    virtual void Merge(TParaBuiltinFunctionTable* Source);
    virtual void RegisterAnonymousClass(TParaObjectPrototype* Prototype);
    virtual void RegisterFunctionId(const std::string& FunctionName, long FunctionId);
    virtual int Execute(long FunctionId, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException);
  protected:
    std::vector<TParaObjectPrototype*> _PrototypeList;
    std::map<long, int> _ClassIdTable;
    std::map<long, int> _MethodIdTable;
    std::map<long, std::string> _MethodNameTable;
};


#endif
