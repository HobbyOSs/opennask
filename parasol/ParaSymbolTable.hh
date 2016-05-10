/* ParaSymbolTable.hh */
/* Created by Enomoto Sanshiro on 27 August 1999. */
/* Last updated by Enomoto Sanshiro on 27 August 2000. */


#ifndef __ParaSymbolTable_hh__
#define __ParaSymbolTable_hh__


#include <string>
#include <vector>
#include <map>
#include "ParaDefs.hh"


class TParaObjectPrototype;
class TParaValue;
class TParaFunction;
class TParaObjectPrototypeTable;
class TParaBuiltinFunctionTable;


class TParaNameTable {
  public:
    static TParaNameTable* GetInstance(void);
    ~TParaNameTable();
    long NameToId(const std::string& Name);
    std::string IdToName(long Id);
  protected:
    TParaNameTable(void);
    static TParaNameTable* _Instance;
  protected:
    std::map<std::string, long> _IdTable;
    long _NextId;
};


class TParaSymbolTable {
  public:
    TParaSymbolTable(TParaObjectPrototypeTable* ObjectPrototypeTable = 0, TParaBuiltinFunctionTable* BuiltinFunctionTable = 0);
    TParaSymbolTable(TParaSymbolTable* SymbolTable, int ImportDepth = 1);
    virtual ~TParaSymbolTable();
    virtual bool IsTypeName(const std::string& Symbol) const;
    virtual TParaValue* CreateObject(const std::string& TypeName, int Length = 0);
    virtual void EnterBlock(void);
    virtual void ExitBlock(void) throw(TScriptException);
    virtual long NameToId(const std::string& Name);
    virtual std::string IdToName(long Id);
    virtual long RegisterVariable(const std::string& Name, const TParaValue& InitialValue);
    virtual long RegisterVariable(const std::string& Name, TParaValue* Variable);
    virtual long RegisterVariable(long VariableId, TParaValue* Variable);
    virtual TParaValue* GetVariable(long VariableId);
    virtual void RegisterFunction(long FunctionId, TParaFunction* Function);
    virtual TParaFunction* GetFunction(long FunctionId);
    virtual long Import(TParaSymbolTable* SymbolTable, int Depth = 1);
    virtual TParaObjectPrototypeTable* ObjectPrototypeTable(void);
    virtual TParaBuiltinFunctionTable* BuiltinFunctionTable(void);
  protected:
    TParaNameTable* _NameTable;
    TParaObjectPrototypeTable* _ObjectPrototypeTable;
    TParaBuiltinFunctionTable* _BuiltinFunctionTable;
    int _CurrentBlockDepth;
    long _VariableCount;
    std::vector<long> _VariableCountList;
    std::vector<std::pair<long, TParaValue*> > _VariableEntryList;
    std::vector<TParaValue*> _GlobalVariableList;
    std::map<long, TParaFunction*>* _FunctionTable;
    std::map<long, TParaFunction*>* _OriginalFunctionTable;
};


#endif
