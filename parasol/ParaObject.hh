/* ParaObject.hh */
/* Created by Enomoto Sanshiro on 24 June 1999. */
/* Last updated by Enomoto Sanshiro on 3 July 2002. */


#ifndef __ParaObject_hh__
#define __ParaObject_hh__

#include <string>
#include <vector>
#include <map>
#include "ParaDefs.hh"
#include "ParaValue.hh"


class TParaObjectPrototype;
class TParaOperator;
class TParaSymbolTable;


class TParaObjectPrototypeTable {
  public:
    TParaObjectPrototypeTable(void);
    virtual ~TParaObjectPrototypeTable();
    virtual void Merge(TParaObjectPrototypeTable* Source);
    virtual void RegisterClass(const std::string& ClassName, TParaObjectPrototype* ObjectPrototype);
    virtual void RegisterClass(TParaObjectPrototype* ObjectPrototype);
    virtual TParaObjectPrototype* CreateInstance(const std::string& ClassName);
    virtual TParaObjectPrototype* ReferenceClass(const std::string& ClassName);
    virtual bool IsRegisteredClassName(const std::string& Name) const;
  protected:
    std::map<std::string, TParaObjectPrototype*> _PrototypeTable;
};


class TParaObjectPrototype {
  public:
    TParaObjectPrototype(const std::string& InternalClassName);
    virtual ~TParaObjectPrototype();
    virtual TParaObjectPrototype* Clone(void) = 0;
    virtual void Construct(const std::string& ClassName, std::vector<TParaValue*>& ArgumentList) noexcept(false);
    virtual void Destruct(void) noexcept(false);
    virtual int MethodIdOf(const std::string& MethodName);
    virtual int InvokeMethod(int MethodId, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    virtual int DispatchMessage(const std::string& Message, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    virtual int GetPropertyOf(const std::string& PropertyName, TParaValue& ReturnValue) noexcept(false);
    virtual TParaValue& EvaluateOperator(TParaOperator* Operator, TParaValue& LeftValue, TParaValue& RightValue, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
    virtual std::string InternalClassName(void) const;
    virtual std::string ObjectName(void) const;
    virtual void SetObjectName(const std::string& ObjectName);
  protected:
    std::string _InternalClassName;
    std::string _ObjectName;
    enum {
	MethodId_Undefined = 0,
	_NumberOfMethods
    };
};


#endif
