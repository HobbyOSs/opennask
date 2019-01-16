/* ParaValue.hh */
/* Created by Enomoto Sanshiro on 1 July 1999. */
/* Last updated by Enomoto Sanshiro on 5 April 2002. */


#ifndef __ParaValue_hh__
#define __ParaValue_hh__

#include <string>
#include <vector>
#include <map>
#include "ParaDefs.hh"


class TParaObjectPrototype;
class TParaListValue;

class TParaVariant {};

class TParaValue {
  public:
    explicit TParaValue(void);
    explicit TParaValue(bool BoolValue);
    explicit TParaValue(long LongValue);
    explicit TParaValue(double DoubleValue);
    explicit TParaValue(const std::string& StringValue);
    explicit TParaValue(TParaObjectPrototype* ObjectValue);
    explicit TParaValue(TParaValue* PointerValue);
    explicit TParaValue(const TParaVariant& VariantValue);
    explicit TParaValue(const TParaListValue& ListValue);
    TParaValue(const TParaValue& Value);
    TParaValue& operator=(const TParaValue& Value);
    virtual ~TParaValue();
    virtual void Destroy(void);
    virtual void SetName(std::string Name);
    virtual void Assign(const TParaValue& Value) noexcept(false);
    virtual bool IsVoid(void) const;
    virtual bool IsBool(void) const;
    virtual bool IsLong(void) const;
    virtual bool IsDouble(void) const;
    virtual bool IsString(void) const;
    virtual bool IsObject(void) const;
    virtual bool IsPointer(void) const;
    virtual bool IsList(void) const;
    virtual bool IsNumeric(void) const;
    virtual bool IsObject(const std::string& InternalClassName) const;
    virtual std::string TypeName(void) const;
    virtual bool AsBool(void) const noexcept(false);
    virtual long AsLong(void) const noexcept(false);
    virtual double AsDouble(void) const noexcept(false);
    virtual std::string AsString(void) const noexcept(false);
    virtual std::string& AsStringReference(void) const noexcept(false);
    virtual TParaObjectPrototype* AsObject(void) const noexcept(false);
    virtual TParaValue* AsPointer(void) const noexcept(false);
    virtual TParaListValue& AsList(void) noexcept(false);
    virtual const TParaListValue& AsConstList(void) const noexcept(false);
    virtual std::vector<TParaValue>& AsValueList(void) noexcept(false);
    virtual void SetLeftValueFlag(void);
    virtual bool IsLeftValue(void) const;
    virtual void SetArrayLength(int ArrayLength);
    virtual int ArrayLength(void) const;
    virtual void SetVariantFlag(void);
    virtual bool IsVariant(void) const;
    virtual void SetArrayFlag(void);
    virtual bool IsArray(void) const;
    virtual void Dump(std::ostream& os) const;
  protected:
    enum TValueType {
        vtVoid, vtBool, vtLong, vtDouble, vtString, 
	vtObject, vtPointer, vtList
    };
    union TPrimitiveValue {
	bool _BoolValue;
        long _LongValue;
        double _DoubleValue;
	std::string* _StringValue;
        TParaObjectPrototype* _ObjectValue;
        TParaValue* _PointerValue;
	TParaListValue* _ListValue;
    };
  protected:
    TPrimitiveValue _PrimitiveValue;
    TValueType _Type;
    int _ArrayLength;
    bool _IsLeftValue;
    bool _IsVariant;
    bool _IsArray;
};



class TParaListValue {
  public:
    TParaListValue(void);
    explicit TParaListValue(unsigned InitialCapacity);
    TParaListValue(const std::vector<TParaValue>& ValueList);
    TParaListValue(const TParaListValue& ListValue);
    TParaListValue& operator=(const TParaListValue& ListValue);
    virtual ~TParaListValue();
    virtual unsigned ListSize(void) const;
    virtual bool HasKeyIndex(void) const;
    virtual void AppendList(const TParaListValue& ListValue);
    virtual unsigned AppendValue(const TParaValue& Value);
    virtual std::vector<TParaValue>& ValueList(void);
    virtual const std::vector<TParaValue>& ConstValueList(void) const;
    virtual const std::vector<std::string>& KeyList(void) const;
    virtual void SetKey(unsigned Index, const std::string& Key);
    virtual std::string KeyOf(unsigned Index);
    virtual unsigned IndexOf(const std::string& Key);
    virtual TParaValue& ValueOf(unsigned Index);
    virtual TParaValue& ValueOf(const std::string& Key);
    virtual std::string AsString(void) const;
    inline TParaValue& operator[] (unsigned Index);
    inline TParaValue& operator[] (std::string& Key);
  protected:
    std::vector<TParaValue> _ValueList;
    mutable std::vector<std::string>* _KeyList;
    mutable std::map<std::string, unsigned>* _KeyIndexTable;
    mutable std::map<unsigned, std::string>* _IndexKeyTable;
};


inline TParaValue& TParaListValue::operator[] (unsigned Index)
{
    return ValueOf(Index);
}

inline TParaValue& TParaListValue::operator[] (std::string& Key)
{
    return ValueOf(Key);
}


#endif
