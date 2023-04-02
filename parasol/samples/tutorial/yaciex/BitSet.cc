/* BitSet.cc */
/* Created by Enomoto Sanshiro on 23 October 2001. */
/* Last updated by Enomoto Sanshiro on 23 October 2001. */


#include <string>
#include <vector>
#include "ParaObject.hh"
#include "BitSet.hh"

using namespace std;



TBitSetObject::TBitSetObject(void)
: TParaObjectPrototype("BitSet")
{
}

TBitSetObject::~TBitSetObject()
{
}

TParaObjectPrototype* TBitSetObject::Clone(void)
{
    return new TBitSetObject();
}

void TBitSetObject::Construct(const string& ClassName, vector<TParaValue*>& ArgumentList) noexcept(false)
{
    if (ArgumentList.size() == 0) {
	_Value = 0;
    }
    else if (ArgumentList[0]->IsLong()) {
	_Value = ArgumentList[0]->AsLong();
    }
    else if (ArgumentList[0]->IsString()) {
	string StringValue = ArgumentList[0]->AsString();
	_Value = StringToInt(StringValue);
    }
    else {
	throw TScriptException("BitSet::BitSet(): invalid argument");
    }
}

int TBitSetObject::DispatchMessage(const string& Message, vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false)
{
    if (Message == "set") {
        ReturnValue = Set(ArgumentList);
    }
    else if (Message == "unset") {
        ReturnValue = Unset(ArgumentList);
    }
    else if (Message == "isset") {
        ReturnValue = IsSet(ArgumentList);
    }
    else if (Message == "asInt") {
        ReturnValue = AsUInt32(ArgumentList);
    }
    else if (Message == "asString") {
        ReturnValue = AsString(ArgumentList);
    }
    else {
	return 0;
    }

    return 1;
}

TParaValue TBitSetObject::Set(vector<TParaValue*>& ArgumentList) noexcept(false)
{
    if ((ArgumentList.size() < 1) || (! ArgumentList[0]->IsLong())) {
	throw TScriptException(
	    "BitSet::set(long): invalid argument"
	);
    }

    long Index = ArgumentList[0]->AsLong();
    if ((Index < 0) || (Index >= _BitLength)) {
	throw TScriptException(
	    "BitSet::set(long): argument out of range"
	);
    }

    _Value |= (0x0001 << Index);

    return TParaValue((long) 0);
}

TParaValue TBitSetObject::Unset(vector<TParaValue*>& ArgumentList) noexcept(false)
{
    if ((ArgumentList.size() < 1) || (! ArgumentList[0]->IsLong())) {
	throw TScriptException(
	    "BitSet::unset(long): invalid argument"
	);
    }

    long Index = ArgumentList[0]->AsLong();
    if ((Index < 0) || (Index >= _BitLength)) {
	throw TScriptException(
	    "BitSet::unset(long): argument out of range"
	);
    }

    _Value &= ~(0x0001 << Index);

    return TParaValue((long) 0);
}

TParaValue TBitSetObject::IsSet(vector<TParaValue*>& ArgumentList) noexcept(false)
{
    if ((ArgumentList.size() < 1) || (! ArgumentList[0]->IsLong())) {
	throw TScriptException(
	    "BitSet::isSet(long): invalid argument"
	);
    }

    long Index = ArgumentList[0]->AsLong();
    if ((Index < 0) || (Index >= _BitLength)) {
	throw TScriptException(
	    "BitSet::isSet(long): argument out of range"
	);
    }

    long Result = (_Value >> Index) & 0x0001;

    return TParaValue(Result);
}

TParaValue TBitSetObject::AsUInt32(vector<TParaValue*>& ArgumentList) noexcept(false)
{
    return TParaValue(_Value);
}

TParaValue TBitSetObject::AsString(vector<TParaValue*>& ArgumentList) noexcept(false)
{
    return TParaValue(IntToString(_Value));
}

std::string TBitSetObject::IntToString(long IntValue)
{
    string StringValue(_BitLength, '0');
    for (int i = 0; i < _BitLength; i++) {
	char BitChar = ((IntValue >> i) & 0x0001) ? '1' : '0';
	StringValue[_BitLength - (i + 1)] = BitChar;
    }

    return StringValue;
}

long TBitSetObject::StringToInt(const std::string& StringValue)
{
    long LongValue = 0;
    for (unsigned i = 0; (i < _BitLength) && (i < StringValue.size()); i++) {
	LongValue <<= 1;
	LongValue |= ((StringValue[i] == '0') ? 0 : 1);
    }

    return LongValue;
}
