/* BitFunction.hh */
/* Created by Enomoto Sanshiro on 23 October 2001. */
/* Last updated by Enomoto Sanshiro on 23 October 2001. */


#ifndef __BitFunction_hh__
#define __BitFunction_hh__


#include <string>
#include <vector>
#include "ParaObject.hh"


class TBitFunction: public TParaObjectPrototype {
  public:
    TBitFunction(void);
    virtual ~TBitFunction();
    virtual TParaObjectPrototype* Clone(void);
    virtual void Construct(const std::string& ClassName, std::vector<TParaValue*>& ArgumentList) throw(TScriptException);
    virtual int DispatchMessage(const std::string& Message, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException);
  protected:
    TParaValue IntToBitString(std::vector<TParaValue*>& ArgumentList) throw(TScriptException);
    TParaValue BitStringToInt(std::vector<TParaValue*>& ArgumentList) throw(TScriptException);
};


#endif
