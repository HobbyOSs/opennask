/* BitSet.hh */
/* Created by Enomoto Sanshiro on 23 October 2001. */
/* Last updated by Enomoto Sanshiro on 23 October 2001. */


#ifndef __BitSet_hh__
#define __BitSet_hh__


#include <string>
#include <vector>
#include "ParaObject.hh"


class TBitSetObject: public TParaObjectPrototype {
  public:
    TBitSetObject(void);
    virtual ~TBitSetObject();
    virtual TParaObjectPrototype* Clone(void);
    virtual void Construct(const std::string& ClassName, std::vector<TParaValue*>& ArgumentList) throw(TScriptException);
    virtual int DispatchMessage(const std::string& Message, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException);
  public:
    static std::string IntToString(long IntValue);
    static long StringToInt(const std::string& StringValue);
  protected:
    TParaValue Set(std::vector<TParaValue*>& ArgumentList) throw(TScriptException);
    TParaValue Unset(std::vector<TParaValue*>& ArgumentList) throw(TScriptException);
    TParaValue IsSet(std::vector<TParaValue*>& ArgumentList) throw(TScriptException);
    TParaValue AsInt(std::vector<TParaValue*>& ArgumentList) throw(TScriptException);
    TParaValue AsString(std::vector<TParaValue*>& ArgumentList) throw(TScriptException);
  private:
    long _Value;
    enum {
	_BitLength = 8 * sizeof(long)
    };
};


#endif
