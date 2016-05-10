/* ParaContainerLibrary.hh */
/* Created by Enomoto Sanshiro on 22 April 2000. */
/* Last updated by Enomoto Sanshiro on 22 April 2000. */


#ifndef __ParaContainerLibrary_hh__
#define __ParaContainerLibrary_hh__


#include <string>
#include <vector>
#include "ParaObject.hh"


// This class will be removed... do not use.

class TParaVectorObject: public TParaObjectPrototype {
  public:
    TParaVectorObject(void);
    virtual ~TParaVectorObject();
    virtual TParaObjectPrototype* Clone(void);
    virtual int DispatchMessage(const std::string& Message, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException);
  protected:
    TParaValue PutAt(std::vector<TParaValue*>& ArgumentList) throw(TScriptException);
    TParaValue GetAt(std::vector<TParaValue*>& ArgumentList) throw(TScriptException);
    TParaValue PushBack(std::vector<TParaValue*>& ArgumentList) throw(TScriptException);
    TParaValue Size(std::vector<TParaValue*>& ArgumentList) throw(TScriptException);
  protected:
    std::vector<TParaValue> _VectorStorage;
};


#endif
