/* BitOperators.cc */
/* Created by Enomoto Sanshiro on 24 August 2000. */
/* Last updated by Enomoto Sanshiro on 24 August 2000. */


#include "ParaOperator.hh"
#include "ParaSymbolTable.hh"
#include "BitOperators.hh"

using namespace std;


TOperatorBitGenerate::TOperatorBitGenerate(void)
{
}

TOperatorBitGenerate::~TOperatorBitGenerate()
{
}

TParaOperator* TOperatorBitGenerate::Clone(void) const
{
    return new TOperatorBitGenerate();
}

string TOperatorBitGenerate::Symbol(void) const
{
    return string("#");
}

string TOperatorBitGenerate::Name(void) const
{
    return string("BitGenerate");
}

TParaValue& TOperatorBitGenerate::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& ReturnValue) throw(TScriptException) 
{
    long BitPosition = Right.AsLong();
    if (BitPosition < 0) {
	throw TScriptException("Natural number was expected.");
    }

    ReturnValue = TParaValue((long) (0x0001ul << BitPosition));

    return ReturnValue;
} 



TOperatorBitFill::TOperatorBitFill(void)
{
}

TOperatorBitFill::~TOperatorBitFill()
{
}

TParaOperator* TOperatorBitFill::Clone(void) const
{
    return new TOperatorBitFill();
}

string TOperatorBitFill::Symbol(void) const
{
    return string("..");
}

string TOperatorBitFill::Name(void) const
{
    return string("BitFill");
}

TParaValue& TOperatorBitFill::Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& ReturnValue) throw(TScriptException) 
{
    unsigned long LongValue;

    if (Left.AsLong() == Right.AsLong()) {
	LongValue = (unsigned long) Left.AsLong();
    }
    else {
	LongValue = (unsigned long) (Left.AsLong() | Right.AsLong());
	bool IsToBeFilled = false;
	for (unsigned long Bit = 0x0001; Bit < LongValue; Bit <<= 1) {
	    if (LongValue & Bit) {
		if (! IsToBeFilled) {
		    IsToBeFilled = true;
		}
		else {
		    break;
		}
	    }
	    if (IsToBeFilled) {
		LongValue |= Bit;
	    }
	}
    }

    ReturnValue = TParaValue((long) LongValue);

    return ReturnValue;
} 

