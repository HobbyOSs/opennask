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
    virtual void Construct(const std::string& ClassName, std::vector<TParaValue*>& ArgumentList) noexcept(false);
    virtual int DispatchMessage(const std::string& Message, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false);
  public:
    static std::string IntToString(long IntValue);
    static long StringToInt(const std::string& StringValue);
  protected:
    TParaValue Set(std::vector<TParaValue*>& ArgumentList) noexcept(false);
    TParaValue Unset(std::vector<TParaValue*>& ArgumentList) noexcept(false);
    TParaValue IsSet(std::vector<TParaValue*>& ArgumentList) noexcept(false);
    TParaValue AsInt(std::vector<TParaValue*>& ArgumentList) noexcept(false);
    TParaValue AsString(std::vector<TParaValue*>& ArgumentList) noexcept(false);
  private:
    long _Value;
    enum {
	_BitLength = 8 * sizeof(long)
    };
};


#endif
