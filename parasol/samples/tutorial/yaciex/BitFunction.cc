/* BitFunction.cc */
/* Created by Enomoto Sanshiro on 23 October 2001. */
/* Last updated by Enomoto Sanshiro on 23 October 2001. */


#include <string>
#include <vector>
#include "ParaObject.hh"
#include "BitSet.hh"
#include "BitFunction.hh"

using namespace std;



TBitFunction::TBitFunction(void)
: TParaObjectPrototype("BitFunction")
{
}

TBitFunction::~TBitFunction()
{
}

TParaObjectPrototype* TBitFunction::Clone(void)
{
    return new TBitFunction();
}

void TBitFunction::Construct(const string& ClassName, vector<TParaValue*>& ArgumentList) noexcept(false)
{
}

int TBitFunction::DispatchMessage(const string& Message, vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) noexcept(false)
{
    if (Message == "bitStringToInt") {
        ReturnValue = BitStringToInt(ArgumentList);
    }
    else if (Message == "intToBitString") {
        ReturnValue = IntToBitString(ArgumentList);
    }
    else {
	return 0;
    }

    return 1;
}

TParaValue TBitFunction::BitStringToInt(vector<TParaValue*>& ArgumentList) noexcept(false)
{
    if ((ArgumentList.size() < 1) || (! ArgumentList[0]->IsString())) {
	throw TScriptException(
	    "bitStringToInt(string): invalid argument"
	);
    }

    string StringValue = ArgumentList[0]->AsString();
    long LongValue = TBitSetObject::StringToInt(StringValue);

    return TParaValue(LongValue);
}

TParaValue TBitFunction::IntToBitString(vector<TParaValue*>& ArgumentList) noexcept(false)
{
    if ((ArgumentList.size() < 1) || (! ArgumentList[0]->IsLong())) {
	throw TScriptException(
	    "intToBitString(long): invalid argument"
	);
    }

    long LongValue = ArgumentList[0]->AsLong();
    string StringValue = TBitSetObject::IntToString(LongValue);

    return TParaValue(StringValue);
}
