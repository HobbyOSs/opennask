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
    virtual int DispatchMessage(const std::string& Message, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
  protected:
    TParaValue PutAt(std::vector<TParaValue*>& ArgumentList) noexcept(false);
    TParaValue GetAt(std::vector<TParaValue*>& ArgumentList) noexcept(false);
    TParaValue PushBack(std::vector<TParaValue*>& ArgumentList) noexcept(false);
    TParaValue Size(std::vector<TParaValue*>& ArgumentList) noexcept(false);
  protected:
    std::vector<TParaValue> _VectorStorage;
};


#endif
