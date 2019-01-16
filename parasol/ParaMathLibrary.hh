/* ParaMathLibrary.hh */
/* Created by Enomoto Sanshiro on 24 June 1999. */
/* Last updated by Enomoto Sanshiro on 12 April 2001. */


#ifndef __ParaMathLibrary_hh__
#define __ParaMathLibrary_hh__


#include <string>
#include <vector>
#include "ParaObject.hh"



class TParaMathObject: public TParaObjectPrototype {
  public:
    TParaMathObject(void);
    virtual ~TParaMathObject();
    virtual TParaObjectPrototype* Clone(void);
    virtual int MethodIdOf(const std::string& MethodName);
    virtual int InvokeMethod(int MethodId, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
  protected:
    enum {
	MethodId_Sin = TParaObjectPrototype::_NumberOfMethods,
	MethodId_Cos,
	MethodId_Tan,
	MethodId_Asin,
	MethodId_Acos,
	MethodId_Atan,
	MethodId_Atan2,
	MethodId_Exp,
	MethodId_Log,
	MethodId_Log10,
	MethodId_Sqrt,
	MethodId_Abs,
	MethodId_Srand,
	MethodId_Rand,
	_NumberOfMethods
    };
  protected:
    int Sin(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Cos(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Tan(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Asin(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Acos(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Atan(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Atan2(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Exp(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Log(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Log10(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Sqrt(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Abs(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Srand(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Rand(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
};



class TParaListMathObject: public TParaObjectPrototype {
  public:
    TParaListMathObject(void);
    virtual ~TParaListMathObject();
    virtual TParaObjectPrototype* Clone(void);
    virtual int MethodIdOf(const std::string& MethodName);
    virtual int InvokeMethod(int MethodId, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
  protected:
    enum {
	MethodId_Length = TParaObjectPrototype::_NumberOfMethods,
	MethodId_Min,
	MethodId_Max,
	MethodId_Sum,
	MethodId_Mean,
	MethodId_Deviation,
	MethodId_Delta,
	MethodId_Sigma,
	MethodId_Find,
	MethodId_FindKeys,
	MethodId_Count,
	_NumberOfMethods
    };
  protected:
    int Length(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Min(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Max(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Sum(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Mean(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Deviation(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Delta(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Sigma(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Find(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int FindKeys(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Count(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
};


#endif
