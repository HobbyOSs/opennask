/* ParaStandardLibrary.hh */
/* Created by Enomoto Sanshiro on 24 June 1999. */
/* Last updated by Enomoto Sanshiro on 24 March 2002. */


#ifndef __ParaStandardLibrary_hh__
#define __ParaStandardLibrary_hh__


#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <set>
#include "ParaObject.hh"


class TParaConsoleObject: public TParaObjectPrototype {
  public:
    TParaConsoleObject(void);
    virtual ~TParaConsoleObject();
    virtual TParaObjectPrototype* Clone(void);
    virtual int MethodIdOf(const std::string& MethodName);
    virtual int InvokeMethod(int MethodId, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
  protected:
    enum {
	MethodId_Print = TParaObjectPrototype::_NumberOfMethods,
	MethodId_PrintLine,
	MethodId_GetLine,
	MethodId_GetByte,
	_NumberOfMethods
    };
  protected:
    int Print(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
    int PrintLine(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
    int GetLine(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
    int GetByte(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
};


class TParaInputFileObject: public TParaObjectPrototype {
  public:
    TParaInputFileObject(void);
    virtual ~TParaInputFileObject();
    virtual TParaObjectPrototype* Clone(void);
    virtual void Construct(const std::string& ClassName, std::vector<TParaValue*>& ArgumentList) noexcept(false);
    virtual int MethodIdOf(const std::string& MethodName);
    virtual int InvokeMethod(int MethodId, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
  protected:
    enum {
	MethodId_GetLine = TParaObjectPrototype::_NumberOfMethods,
	MethodId_GetByte,
	_NumberOfMethods
    };
  protected:
    virtual int GetLine(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
    virtual int GetByte(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
  private:
    std::ifstream* _FileStream;
};


class TParaOutputFileObject: public TParaObjectPrototype {
  public:
    TParaOutputFileObject(void);
    virtual ~TParaOutputFileObject();
    virtual TParaObjectPrototype* Clone(void);
    virtual void Construct(const std::string& ClassName, std::vector<TParaValue*>& ArgumentList) noexcept(false);
    virtual int MethodIdOf(const std::string& MethodName);
    virtual int InvokeMethod(int MethodId, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
  protected:
    enum {
	MethodId_Print = TParaObjectPrototype::_NumberOfMethods,
	MethodId_PrintLine,
	_NumberOfMethods
    };
  protected:
    virtual int Print(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
    virtual int PrintLine(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
  private:
    std::ofstream* _FileStream;
};


class TParaInputPipeObject: public TParaInputFileObject {
  public:
    TParaInputPipeObject(void);
    virtual ~TParaInputPipeObject();
    virtual TParaObjectPrototype* Clone(void);
    virtual void Construct(const std::string& ClassName, std::vector<TParaValue*>& ArgumentList) noexcept(false);
  protected:
    virtual int GetLine(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
    virtual int GetByte(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
  private:
    std::FILE* _Pipe;
};


class TParaOutputPipeObject: public TParaOutputFileObject {
  public:
    TParaOutputPipeObject(void);
    virtual ~TParaOutputPipeObject();
    virtual TParaObjectPrototype* Clone(void);
    virtual void Construct(const std::string& ClassName, std::vector<TParaValue*>& ArgumentList) noexcept(false);
  protected:
    virtual int Print(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
    virtual int PrintLine(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
  private:
    std::FILE* _Pipe;
};


class TParaFormatterObject: public TParaObjectPrototype {
  public:
    TParaFormatterObject(void);
    virtual ~TParaFormatterObject();
    virtual TParaObjectPrototype* Clone(void);
    virtual int MethodIdOf(const std::string& MethodName);
    virtual int InvokeMethod(int MethodId, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
  protected:
    enum {
	MethodId_Put = TParaObjectPrototype::_NumberOfMethods,
	MethodId_Flush,
	MethodId_SetWidth,
	MethodId_SetPrecision,
	MethodId_SetFill,
	MethodId_SetBase,
	MethodId_Hex,
	MethodId_Dec,
	MethodId_Fixed,
	MethodId_Scientific,
	_NumberOfMethods
    };
  protected:
    int Put(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Flush(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int SetWidth(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int SetPrecision(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int SetFill(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int SetBase(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Hex(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Dec(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Fixed(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Scientific(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
  private:
    std::ostringstream* _FormatStream;
};


class TParaScannerObject: public TParaObjectPrototype {
  public:
    TParaScannerObject(void);
    virtual ~TParaScannerObject();
    virtual TParaObjectPrototype* Clone(void);
    virtual void Construct(const std::string& ClassName, std::vector<TParaValue*>& ArgumentList) noexcept(false);
    virtual int MethodIdOf(const std::string& MethodName);
    virtual int InvokeMethod(int MethodId, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
  protected:
    enum {
	MethodId_Load = TParaObjectPrototype::_NumberOfMethods,
	MethodId_Get,
	MethodId_GetLine,
	MethodId_SkipWhiteSpace,
	MethodId_SetBase,
	MethodId_IsGood,
	MethodId_LastGetCount,
	_NumberOfMethods
    };
  protected:
    int Load(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int Get(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int GetLine(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int SkipWhiteSpace(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int SetBase(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int IsGood(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
    int LastGetCount(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
  private:
    std::istringstream* _SourceStream;
};


class TParaArgumentObject: public TParaObjectPrototype {
  public:
    TParaArgumentObject(int argc, char** argv);
    virtual ~TParaArgumentObject();
    virtual TParaObjectPrototype* Clone(void);
    virtual int MethodIdOf(const std::string& MethodName);
    virtual int InvokeMethod(int MethodId, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
  protected:
    virtual void Parse(void);
  protected:
    enum {
	MethodId_NumberOfArguments = TParaObjectPrototype::_NumberOfMethods,
	MethodId_GetArgumentOf,
	MethodId_NumberOfParameters,
	MethodId_GetParameterOf,
	MethodId_IsOptionSpecified,
	MethodId_GetOptionValueOf,
	MethodId_IsSwitchSpecified,
	_NumberOfMethods
    };
  protected:
    int NumberOfArguments(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
    int GetArgumentOf(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
    int NumberOfParameters(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
    int GetParameterOf(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
    int IsOptionSpecified(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
    int GetOptionValueOf(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
    int IsSwitchSpecified(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
  private:
    int _argc;
    char** _argv;
    bool _IsParsed;
    std::vector<std::string> _ArgumentList;
    std::vector<std::string> _ParameterList;
    std::map<std::string, std::string> _OptionTable;
    std::set<char> _SwitchSet;
};


class TParaStringObject: public TParaObjectPrototype {
  public:
    TParaStringObject(void);
    virtual ~TParaStringObject();
    virtual TParaObjectPrototype* Clone(void);
    virtual int MethodIdOf(const std::string& MethodName);
    virtual int InvokeMethod(int MethodId, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
  protected:
    int Chop(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
    int Chomp(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
    int Substr(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
    int Index(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
  protected:
    enum {
	MethodId_Chop = TParaObjectPrototype::_NumberOfMethods,
	MethodId_Chomp,
	MethodId_Substr,
	MethodId_Index,
	_NumberOfMethods
    };
};


class TParaSystemObject: public TParaObjectPrototype {
  public:
    TParaSystemObject(void);
    virtual ~TParaSystemObject();
    virtual TParaObjectPrototype* Clone(void);
    virtual int MethodIdOf(const std::string& MethodName);
    virtual int InvokeMethod(int MethodId, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
  protected:
    int System(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
    int Shell(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) noexcept(false);
  protected:
    enum {
	MethodId_System = TParaObjectPrototype::_NumberOfMethods,
	MethodId_Shell,
	_NumberOfMethods
    };
};


#endif
