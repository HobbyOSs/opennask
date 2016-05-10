/* ParaMathLibrary.cc */
/* Created by Enomoto Sanshiro on 22 August 1999. */
/* Last updated by Enomoto Sanshiro on 12 April 2001. */


#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <cstdlib>
#include "ParaObject.hh"
#include "ParaMathLibrary.hh"

using namespace std;


template<class T> inline static T sqr(const T& x) { return x*x; }


TParaMathObject::TParaMathObject(void)
: TParaObjectPrototype("Math")
{
}

TParaMathObject::~TParaMathObject()
{
}

TParaObjectPrototype* TParaMathObject::Clone(void)
{
    return new TParaMathObject();
}

int TParaMathObject::MethodIdOf(const std::string& MethodName)
{
    if (MethodName == "sin") {
        return MethodId_Sin;
    }
    else if (MethodName == "cos") {
        return MethodId_Cos;
    }
    else if (MethodName == "tan") {
        return MethodId_Tan;
    }
    else if (MethodName == "asin") {
        return MethodId_Asin;
    }
    else if (MethodName == "acos") {
        return MethodId_Acos;
    }
    else if (MethodName == "atan") {
        return MethodId_Atan;
    }
    else if (MethodName == "atan2") {
        return MethodId_Atan2;
    }
    else if (MethodName == "exp") {
        return MethodId_Exp;
    }
    else if (MethodName == "log") {
        return MethodId_Log;
    }
    else if (MethodName == "log10") {
        return MethodId_Log10;
    }
    else if (MethodName == "sqrt") {
        return MethodId_Sqrt;
    }
    else if (MethodName == "abs") {
        return MethodId_Abs;
    }
    else if (MethodName == "srand") {
        return MethodId_Srand;
    }
    else if (MethodName == "rand") {
        return MethodId_Rand;
    }

    return TParaObjectPrototype::MethodIdOf(MethodName);
}

int TParaMathObject::InvokeMethod(int MethodId, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    int Result = 0;

    switch (MethodId) {
      case MethodId_Sin:
        Result = Sin(ArgumentList, ReturnValue);
        break;
      case MethodId_Cos:
        Result = Cos(ArgumentList, ReturnValue);
        break;
      case MethodId_Tan:
        Result = Tan(ArgumentList, ReturnValue);
        break;
      case MethodId_Asin:
        Result = Asin(ArgumentList, ReturnValue);
        break;  
      case MethodId_Acos:
        Result = Acos(ArgumentList, ReturnValue);
        break;
      case MethodId_Atan:
        Result = Atan(ArgumentList, ReturnValue);
        break;
      case MethodId_Atan2:
        Result = Atan2(ArgumentList, ReturnValue);
        break;
      case MethodId_Exp:
        Result = Exp(ArgumentList, ReturnValue);
        break;
      case MethodId_Log:
        Result = Log(ArgumentList, ReturnValue);
        break;
      case MethodId_Log10:
        Result = Log10(ArgumentList, ReturnValue);
        break;
      case MethodId_Sqrt:
        Result = Sqrt(ArgumentList, ReturnValue);
        break;
      case MethodId_Abs:
        Result = Abs(ArgumentList, ReturnValue);
        break;
      case MethodId_Srand:
        Result = Srand(ArgumentList, ReturnValue);
        break;
      case MethodId_Rand:
        Result = Rand(ArgumentList, ReturnValue);
        break;
      default:
	Result = 0;
    }

    return Result;
}

int TParaMathObject::Sin(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (ArgumentList.size() != 1) {
        throw TScriptException("sin(): invalid number of argument[s]");
    }
    
    if (! ArgumentList[0]->IsList()) {
	ReturnValue = TParaValue(sin(ArgumentList[0]->AsDouble()));
    }
    else {
	ReturnValue = TParaValue(TParaListValue());
	vector<TParaValue>& InputList = ArgumentList[0]->AsValueList();
	vector<TParaValue>& ResultList = ReturnValue.AsValueList();
	for (unsigned i = 0; i < InputList.size(); i++) {
	    double x = InputList[i].AsDouble();
	    ResultList.push_back(TParaValue((double) sin(x)));
	}
    }

    return 1;
}

int TParaMathObject::Cos(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (ArgumentList.size() != 1) {
        throw TScriptException("cos(): invalid number of argument[s]");
    }
    
    if (! ArgumentList[0]->IsList()) {
	ReturnValue = TParaValue(cos(ArgumentList[0]->AsDouble()));
    }
    else {
	ReturnValue = TParaValue(TParaListValue());
	vector<TParaValue>& InputList = ArgumentList[0]->AsValueList();
	vector<TParaValue>& ResultList = ReturnValue.AsValueList();
	for (unsigned i = 0; i < InputList.size(); i++) {
	    double x = InputList[i].AsDouble();
	    ResultList.push_back(TParaValue((double) cos(x)));
	}
    }

    return 1;
}

int TParaMathObject::Tan(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (ArgumentList.size() != 1) {
        throw TScriptException("tan(): invalid number of argument[s]");
    }
    
    if (! ArgumentList[0]->IsList()) {
	double x = ArgumentList[0]->AsDouble();
	if (cos(x) == 0) {
	    throw TScriptException("tan(): invalid argument");
	}
	ReturnValue = TParaValue(tan(ArgumentList[0]->AsDouble()));
    }
    else {
	ReturnValue = TParaValue(TParaListValue());
	vector<TParaValue>& InputList = ArgumentList[0]->AsValueList();
	vector<TParaValue>& ResultList = ReturnValue.AsValueList();
	for (unsigned i = 0; i < InputList.size(); i++) {
	    double x = InputList[i].AsDouble();
	    if (cos(x) == 0) {
		throw TScriptException("tan(): invalid argument");
	    }
	    double Value = tan(x);
	    ResultList.push_back(TParaValue(Value));
	}
    }

    return 1;
}

int TParaMathObject::Asin(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (ArgumentList.size() != 1) {
        throw TScriptException("asin(): invalid number of argument[s]");
    }
    
    if (! ArgumentList[0]->IsList()) {
	double x = ArgumentList[0]->AsDouble();
	if ((x < -1.0) || (x > 1.0)) {
	    throw TScriptException("asin(): invalid argument");
	}
	ReturnValue = TParaValue((double) asin(x));
    }
    else {
	ReturnValue = TParaValue(TParaListValue());
	vector<TParaValue>& InputList = ArgumentList[0]->AsValueList();
	vector<TParaValue>& ResultList = ReturnValue.AsValueList();
	for (unsigned i = 0; i < InputList.size(); i++) {
	    double x = InputList[i].AsDouble();
	    if ((x < -1.0) || (x > 1.0)) {
		throw TScriptException("asin(): invalid argument");
	    }
	    ResultList.push_back(TParaValue((double) asin(x)));
	}
    }

    return 1;
}

int TParaMathObject::Acos(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (ArgumentList.size() != 1) {
        throw TScriptException("acos(): invalid number of argument[s]");
    }
    
    if (! ArgumentList[0]->IsList()) {
	double x = ArgumentList[0]->AsDouble();
	if ((x < -1.0) || (x > 1.0)) {
	    throw TScriptException("acos(): invalid argument");
	}
	ReturnValue = TParaValue((double) acos(x));
    }
    else {
	ReturnValue = TParaValue(TParaListValue());
	vector<TParaValue>& InputList = ArgumentList[0]->AsValueList();
	vector<TParaValue>& ResultList = ReturnValue.AsValueList();
	for (unsigned i = 0; i < InputList.size(); i++) {
	    double x = InputList[i].AsDouble();
	    if ((x < -1.0) || (x > 1.0)) {
		throw TScriptException("acos(): invalid argument");
	    }
	    ResultList.push_back(TParaValue((double) acos(x)));
	}
    }

    return 1;
}

int TParaMathObject::Atan(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (ArgumentList.size() != 1) {
        throw TScriptException("atan(): invalid number of argument[s]");
    }
    
    if (! ArgumentList[0]->IsList()) {
	ReturnValue = TParaValue(atan(ArgumentList[0]->AsDouble()));
    }
    else {
	ReturnValue = TParaValue(TParaListValue());
	vector<TParaValue>& InputList = ArgumentList[0]->AsValueList();
	vector<TParaValue>& ResultList = ReturnValue.AsValueList();
	for (unsigned i = 0; i < InputList.size(); i++) {
	    double x = InputList[i].AsDouble();
	    ResultList.push_back(TParaValue((double) atan(x)));
	}
    }

    return 1;
}

int TParaMathObject::Atan2(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (ArgumentList.size() != 2) {
        throw TScriptException(
	    "atan2(double y, double x): invalid number of argument[s]"
	);
    }
    
    if ((! ArgumentList[0]->IsList()) && (! ArgumentList[1]->IsList())) {
	double y = ArgumentList[0]->AsDouble();
	double x = ArgumentList[1]->AsDouble();
	ReturnValue = TParaValue(atan2(y, x));
    }
    else {
	vector<TParaValue>& YList = ArgumentList[0]->AsValueList();
	vector<TParaValue>& XList = ArgumentList[1]->AsValueList();
	if (YList.size() != XList.size()) {
	    throw TScriptException(
		"atan2(): inconsistent length of argument lists"
	    );
	}

	ReturnValue = TParaValue(TParaListValue());
	vector<TParaValue>& ResultList = ReturnValue.AsValueList();
	for (unsigned i = 0; i < XList.size(); i++) {
	    double y = YList[i].AsDouble();
	    double x = XList[i].AsDouble();
	    ResultList.push_back(TParaValue((double) atan2(y, x)));
	}
    }

    return 1;
}

int TParaMathObject::Exp(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (ArgumentList.size() != 1) {
        throw TScriptException("exp(): invalid number of argument[s]");
    }
    
    if (! ArgumentList[0]->IsList()) {
	ReturnValue = TParaValue(exp(ArgumentList[0]->AsDouble()));
    }
    else {
	ReturnValue = TParaValue(TParaListValue());
	vector<TParaValue>& InputList = ArgumentList[0]->AsValueList();
	vector<TParaValue>& ResultList = ReturnValue.AsValueList();
	for (unsigned i = 0; i < InputList.size(); i++) {
	    double x = InputList[i].AsDouble();
	    ResultList.push_back(TParaValue((double) exp(x)));
	}
    }

    return 1;
}

int TParaMathObject::Log(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (ArgumentList.size() != 1) {
        throw TScriptException("log(): invalid number of argument[s]");
    }

    if (! ArgumentList[0]->IsList()) {
	double x = ArgumentList[0]->AsDouble();
	if (x <= 0) {
	    throw TScriptException("log(): invalid argument");
	}
	ReturnValue = TParaValue((double) log(x));
    }
    else {
	ReturnValue = TParaValue(TParaListValue());
	vector<TParaValue>& InputList = ArgumentList[0]->AsValueList();
	vector<TParaValue>& ResultList = ReturnValue.AsValueList();
	for (unsigned i = 0; i < InputList.size(); i++) {
	    double x = InputList[i].AsDouble();
	    if (x <= 0) {
		throw TScriptException("log(): invalid argument");
	    }
	    ResultList.push_back(TParaValue((double) log(x)));
	}
    }

    return 1;
}

int TParaMathObject::Log10(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (ArgumentList.size() != 1) {
        throw TScriptException("log10(): invalid number of argument[s]");
    }

    if (! ArgumentList[0]->IsList()) {
	double x = ArgumentList[0]->AsDouble();
	if (x <= 0) {
	    throw TScriptException("log10(): invalid argument");
	}
	ReturnValue = TParaValue((double) log10(x));
    }
    else {
	ReturnValue = TParaValue(TParaListValue());
	vector<TParaValue>& InputList = ArgumentList[0]->AsValueList();
	vector<TParaValue>& ResultList = ReturnValue.AsValueList();
	for (unsigned i = 0; i < InputList.size(); i++) {
	    double x = InputList[i].AsDouble();
	    if (x <= 0) {
		throw TScriptException("log10(): invalid argument");
	    }
	    ResultList.push_back(TParaValue((double) log10(x)));
	}
    }

    return 1;
}

int TParaMathObject::Sqrt(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (ArgumentList.size() != 1) {
        throw TScriptException("sqrt(): invalid number of argument[s]");
    }
    
    if (! ArgumentList[0]->IsList()) {
	double x = ArgumentList[0]->AsDouble();
	if (x < 0) {
	    throw TScriptException("sqrt(): invalid argument");
	}
	ReturnValue = TParaValue((double) sqrt(x));
    }
    else {
	ReturnValue = TParaValue(TParaListValue());
	vector<TParaValue>& InputList = ArgumentList[0]->AsValueList();
	vector<TParaValue>& ResultList = ReturnValue.AsValueList();
	for (unsigned i = 0; i < InputList.size(); i++) {
	    double x = InputList[i].AsDouble();
	    if (x < 0) {
		throw TScriptException("sqrt(): invalid argument");
	    }
	    ResultList.push_back(TParaValue((double) sqrt(x)));
	}
    }

    return 1;
}

int TParaMathObject::Abs(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (ArgumentList.size() != 1) {
        throw TScriptException("abs(): invalid number of argument[s]");
    }
    
    if (! ArgumentList[0]->IsList()) {
	ReturnValue = TParaValue(fabs(ArgumentList[0]->AsDouble()));
    }
    else {
	ReturnValue = TParaValue(TParaListValue());
	vector<TParaValue>& InputList = ArgumentList[0]->AsValueList();
	vector<TParaValue>& ResultList = ReturnValue.AsValueList();
	for (unsigned i = 0; i < InputList.size(); i++) {
	    double x = InputList[i].AsDouble();
	    ResultList.push_back(TParaValue((double) fabs(x)));
	}
    }

    return 1;
}

int TParaMathObject::Srand(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if ((ArgumentList.size() != 1) || ! (ArgumentList[0]->IsLong())) {
        throw TScriptException("srand(int): invalid number of argument[s]");
    }

    srand48(ArgumentList[0]->AsLong());

    return 1;
}

int TParaMathObject::Rand(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (ArgumentList.size() == 0) {
	ReturnValue = TParaValue((double) drand48());
    }
    else if (ArgumentList[0]->IsList()) {
	ReturnValue = TParaValue(TParaListValue());
	vector<TParaValue>& InputList = ArgumentList[0]->AsValueList();
	vector<TParaValue>& ResultList = ReturnValue.AsValueList();
	for (unsigned i = 0; i < InputList.size(); i++) {
	    ResultList.push_back(TParaValue((double) drand48()));
	}
    }
    else {
        throw TScriptException("rand(): invalid argument[s]");
    }

    return 1;
}



TParaListMathObject::TParaListMathObject(void)
: TParaObjectPrototype("ListMath")
{
}

TParaListMathObject::~TParaListMathObject()
{
}

TParaObjectPrototype* TParaListMathObject::Clone(void)
{
    return new TParaListMathObject();
}

int TParaListMathObject::MethodIdOf(const std::string& MethodName)
{
    if (MethodName == "length") {
        return MethodId_Length;
    }
    else if (MethodName == "min") {
        return MethodId_Min;
    }
    else if (MethodName == "max") {
        return MethodId_Max;
    }
    else if (MethodName == "sum") {
        return MethodId_Sum;
    }
    else if (MethodName == "mean") {
        return MethodId_Mean;
    }
    else if ((MethodName == "deviation") || (MethodName == "rms")) {
        return MethodId_Deviation;
    }
    else if (MethodName == "delta") {
        return MethodId_Delta;
    }
    else if (MethodName == "sigma") {
        return MethodId_Sigma;
    }
    else if (MethodName == "find") {
        return MethodId_Find;
    }
    else if ((MethodName == "findkeys") || (MethodName == "findKeys")) {
        return MethodId_FindKeys;
    }
    else if (MethodName == "count") {
        return MethodId_Count;
    }

    return TParaObjectPrototype::MethodIdOf(MethodName);
}

int TParaListMathObject::InvokeMethod(int MethodId, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    int Result = 0;

    switch (MethodId) {
      case MethodId_Length:
        Result = Length(ArgumentList, ReturnValue);
        break;
      case MethodId_Min:
        Result = Min(ArgumentList, ReturnValue);
        break;
      case MethodId_Max:
        Result = Max(ArgumentList, ReturnValue);
        break;
      case MethodId_Sum:
        Result = Sum(ArgumentList, ReturnValue);
        break;
      case MethodId_Mean:
        Result = Mean(ArgumentList, ReturnValue);
        break;
      case MethodId_Deviation:
        Result = Deviation(ArgumentList, ReturnValue);
        break;
      case MethodId_Delta:
        Result = Delta(ArgumentList, ReturnValue);
        break;
      case MethodId_Sigma:
        Result = Sigma(ArgumentList, ReturnValue);
        break;
      case MethodId_Find:
        Result = Find(ArgumentList, ReturnValue);
        break;
      case MethodId_FindKeys:
	Result = FindKeys(ArgumentList, ReturnValue);
        break;
      case MethodId_Count:
        Result = Count(ArgumentList, ReturnValue);
        break;
      default:
	Result = 0;
    }

    return Result;
}

int TParaListMathObject::Length(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if ((ArgumentList.size() != 1) || (! ArgumentList[0]->IsList())) {
        throw TScriptException("length(): invalid argument[s]");
    }
    
    const TParaListValue& ListValue = ArgumentList[0]->AsConstList();
    const vector<TParaValue>& ValueList = ListValue.ConstValueList();
    unsigned ListLength = ValueList.size();

    ReturnValue = TParaValue((long) ListLength);

    return 1;
}

int TParaListMathObject::Min(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if ((ArgumentList.size() != 1) || (! ArgumentList[0]->IsList())) {
        throw TScriptException("min(): invalid argument[s]");
    }
    
    const TParaListValue& ListValue = ArgumentList[0]->AsConstList();
    const vector<TParaValue>& ValueList = ListValue.ConstValueList();
    unsigned ListLength = ValueList.size();

    if (ListLength == 0) {
        throw TScriptException("min(): empty list");
    }

    double Value = ValueList[0].AsDouble();
    for (unsigned i = 1; i < ListLength; i++) {
        Value = min(Value, ValueList[i].AsDouble());
    }

    ReturnValue = TParaValue(Value);

    return 1;
}

int TParaListMathObject::Max(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if ((ArgumentList.size() != 1) || (! ArgumentList[0]->IsList())) {
        throw TScriptException("max(): invalid argument[s]");
    }
    
    const TParaListValue& ListValue = ArgumentList[0]->AsConstList();
    const vector<TParaValue>& ValueList = ListValue.ConstValueList();
    unsigned ListLength = ValueList.size();

    if (ListLength == 0) {
        throw TScriptException("max(): empty list");
    }

    double Value = ValueList[0].AsDouble();
    for (unsigned i = 1; i < ListLength; i++) {
        Value = max(Value, ValueList[i].AsDouble());
    }

    ReturnValue = TParaValue(Value);

    return 1;
}

int TParaListMathObject::Sum(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if ((ArgumentList.size() != 1) || (! ArgumentList[0]->IsList())) {
        throw TScriptException("sum(): invalid argument[s]");
    }
    
    const TParaListValue& ListValue = ArgumentList[0]->AsConstList();
    const vector<TParaValue>& ValueList = ListValue.ConstValueList();
    unsigned ListLength = ValueList.size();

    double Value = 0;
    for (unsigned i = 0; i < ListLength; i++) {
	Value += ValueList[i].AsDouble();
    }

    ReturnValue = TParaValue(Value);

    return 1;
}

int TParaListMathObject::Mean(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if ((ArgumentList.size() != 1) || (! ArgumentList[0]->IsList())) {
        throw TScriptException("mean(): invalid argument[s]");
    }
    
    const TParaListValue& ListValue = ArgumentList[0]->AsConstList();
    const vector<TParaValue>& ValueList = ListValue.ConstValueList();
    unsigned ListLength = ValueList.size();

    if (ListLength == 0) {
        throw TScriptException("mean(): empty list");
    }

    double Value = 0;
    for (unsigned i = 0; i < ValueList.size(); i++) {
	Value += ValueList[i].AsDouble();
    }
    Value /= ValueList.size();

    ReturnValue = TParaValue(Value);

    return 1;
}

int TParaListMathObject::Deviation(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if ((ArgumentList.size() != 1) || (! ArgumentList[0]->IsList())) {
        throw TScriptException("deviation(): invalid argument[s]");
    }
    
    const TParaListValue& ListValue = ArgumentList[0]->AsConstList();
    const vector<TParaValue>& ValueList = ListValue.ConstValueList();
    unsigned ListLength = ValueList.size();

    if (ListLength == 0) {
        throw TScriptException("deviation(): empty list");
    }

    double Sum = 0, SumOfSquared = 0;
    for (unsigned i = 0; i < ValueList.size(); i++) {
	Sum += ValueList[i].AsDouble();
	SumOfSquared += sqr(ValueList[i].AsDouble());
    }
    double SquaredMean = sqr(Sum / ListLength);
    double MeanOfSquared = SumOfSquared / ListLength;
    double Value = sqrt(MeanOfSquared - SquaredMean);

    ReturnValue = TParaValue(Value);

    return 1;
}

int TParaListMathObject::Delta(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if ((ArgumentList.size() != 1) || (! ArgumentList[0]->IsList())) {
        throw TScriptException("delta(): invalid argument[s]");
    }
    
    ReturnValue = TParaValue(TParaListValue());
    vector<TParaValue>& InputList = ArgumentList[0]->AsValueList();
    vector<TParaValue>& ResultList = ReturnValue.AsValueList();

    bool IsLong = InputList[0].IsLong();
    for (unsigned i = 1; i < InputList.size(); i++) {
	if (IsLong && ! InputList[i].IsLong()) {
	    IsLong = false;
	}
	if (IsLong) {
	    ResultList.push_back(TParaValue(
	        InputList[i].AsLong() - InputList[i-1].AsLong()
	    ));
	}
	else {
	    ResultList.push_back(TParaValue(
	        InputList[i].AsDouble() - InputList[i-1].AsDouble()
	    ));
	}
    }

    return 1;
}

int TParaListMathObject::Sigma(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if ((ArgumentList.size() != 1) || (! ArgumentList[0]->IsList())) {
        throw TScriptException("sigma(): invalid argument[s]");
    }
    
    ReturnValue = TParaValue(TParaListValue());
    vector<TParaValue>& InputList = ArgumentList[0]->AsValueList();
    vector<TParaValue>& ResultList = ReturnValue.AsValueList();

    ResultList.push_back(TParaValue((long) 0));

    bool IsLong = true;
    long LongSum = 0;
    double DoubleSum = 0;
    for (unsigned i = 0; i < InputList.size(); i++) {
	if (IsLong && ! InputList[i].IsLong()) {
	    DoubleSum = LongSum;
	    IsLong = false;
	}
	if (IsLong) {
	    LongSum += InputList[i].AsLong();
	    ResultList.push_back(TParaValue(LongSum));
	}
	else {
	    DoubleSum += InputList[i].AsDouble();
	    ResultList.push_back(TParaValue(DoubleSum));
	}
    }

    return 1;
}

int TParaListMathObject::Find(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if ((ArgumentList.size() != 1) || (! ArgumentList[0]->IsList())) {
        throw TScriptException("find(): invalid argument[s]");
    }
    
    ReturnValue = TParaValue(TParaListValue());
    vector<TParaValue>& ResultList = ReturnValue.AsValueList();

    vector<TParaValue>& InputList = ArgumentList[0]->AsValueList();
    for (unsigned i = 0; i < InputList.size(); i++) {
	if (InputList[i].AsBool()) {
	    ResultList.push_back(TParaValue((long) i));
	}
    }

    return 1;
}

int TParaListMathObject::FindKeys(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if ((ArgumentList.size() != 1) || (! ArgumentList[0]->IsList())) {
        throw TScriptException("findkeys(): invalid argument[s]");
    }
    
    ReturnValue = TParaValue(TParaListValue());
    vector<TParaValue>& ResultList = ReturnValue.AsValueList();

    vector<TParaValue>& InputList = ArgumentList[0]->AsValueList();
    TParaListValue& InputListValue = ArgumentList[0]->AsList();
    for (unsigned i = 0; i < InputList.size(); i++) {
	if (InputList[i].AsBool()) {
	    ResultList.push_back(TParaValue(InputListValue.KeyOf(i)));
	}
    }

    return 1;
}

int TParaListMathObject::Count(std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if ((ArgumentList.size() != 1) || (! ArgumentList[0]->IsList())) {
        throw TScriptException("count(): invalid argument[s]");
    }
    
    long Result = 0;

    vector<TParaValue>& InputList = ArgumentList[0]->AsValueList();
    for (unsigned i = 0; i < InputList.size(); i++) {
	if (InputList[i].AsBool()) {
	    Result++;
	}
    }

    ReturnValue = TParaValue(Result);

    return 1;
}
