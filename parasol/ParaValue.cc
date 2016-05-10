/* ParaValue.cc */
/* Created by Enomoto Sanshiro on 1 July 1999. */
/* Last updated by Enomoto Sanshiro on 5 April 2002. */


#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include "ParaDefs.hh"
#include "ParaObject.hh"
#include "ParaValue.hh"

using namespace std;


TParaValue::TParaValue(void)
{
    _Type = vtVoid;

    _ArrayLength = 0;
    _IsVariant = false;
    _IsLeftValue = false;
    _IsArray = false;
}

TParaValue::TParaValue(bool BoolValue)
{
    _Type = vtBool;
    _PrimitiveValue._BoolValue = BoolValue;

    _ArrayLength = 0;
    _IsVariant = false;
    _IsLeftValue = false;
    _IsArray = false;
}

TParaValue::TParaValue(long LongValue)
{
    _Type = vtLong;
    _PrimitiveValue._LongValue = LongValue;

    _ArrayLength = 0;
    _IsVariant = false;
    _IsLeftValue = false;
    _IsArray = false;
}

TParaValue::TParaValue(double DoubleValue)
{
    _Type = vtDouble;
    _PrimitiveValue._DoubleValue = DoubleValue;

    _ArrayLength = 0;
    _IsLeftValue = false;
    _IsVariant = false;
    _IsArray = false;
}

TParaValue::TParaValue(const string& StringValue)
{
    _Type = vtString;
    _PrimitiveValue._StringValue = new string(StringValue);

    _ArrayLength = 0;
    _IsLeftValue = false;
    _IsVariant = false;
    _IsArray = false;
}

TParaValue::TParaValue(TParaObjectPrototype* ObjectValue)
{
    _Type = vtObject;
    _PrimitiveValue._ObjectValue = ObjectValue;

    _ArrayLength = 0;
    _IsLeftValue = false;
    _IsVariant = false;
    _IsArray = false;
}

TParaValue::TParaValue(TParaValue* PointerValue)
{
    _Type = vtPointer;
    _PrimitiveValue._PointerValue = PointerValue;

    _ArrayLength = 0;
    _IsLeftValue = false;
    _IsVariant = false;
    _IsArray = false;
}

TParaValue::TParaValue(const TParaVariant& VariantValue)
{
    _Type = vtVoid;

    _ArrayLength = 0;
    _IsLeftValue = false;
    _IsVariant = true;
    _IsArray = false;
}

TParaValue::TParaValue(const TParaListValue& ListValue)
{
    _Type = vtList;
    _PrimitiveValue._ListValue = new TParaListValue(ListValue);

    _ArrayLength = 0;
    _IsLeftValue = false;
    _IsVariant = false;
    _IsArray = false;
}

TParaValue::TParaValue(const TParaValue& Value)
{
    _Type = Value._Type;
    if (_Type == vtString) {
	_PrimitiveValue._StringValue = new string(
	    *Value._PrimitiveValue._StringValue
	);
    }
    else if (_Type == vtList) {
	_PrimitiveValue._ListValue = new TParaListValue(
	    *Value._PrimitiveValue._ListValue
	);
    }
    else {
	_PrimitiveValue = Value._PrimitiveValue;
    }

    _ArrayLength = Value._ArrayLength;
    _IsLeftValue = Value._IsLeftValue;
    _IsVariant = Value._IsVariant;
    _IsArray = Value._IsArray;
}

TParaValue& TParaValue::operator=(const TParaValue& Value)
{
    if (_Type != Value._Type) {
	if (_Type == vtString) {
	    delete _PrimitiveValue._StringValue;
	}
	else if (_Type == vtList) {
	    delete _PrimitiveValue._ListValue;
	}
	_Type = Value._Type;

	if (Value._Type == vtString) {
	    _PrimitiveValue._StringValue = new string(
		*Value._PrimitiveValue._StringValue
	    );
	}
	else if (Value._Type == vtList) {
	    _PrimitiveValue._ListValue = new TParaListValue(
		*Value._PrimitiveValue._ListValue
	    );
	}
	else {
	    _PrimitiveValue = Value._PrimitiveValue;
	}
    }
    else {
	if (_Type == vtString) {
	    *_PrimitiveValue._StringValue = (
		*Value._PrimitiveValue._StringValue
            );
	}
	else if (_Type == vtList) {
	    *_PrimitiveValue._ListValue = (
		*Value._PrimitiveValue._ListValue
	    );
	}
	else {
	    _PrimitiveValue = Value._PrimitiveValue;
	}
    }

    _ArrayLength = Value._ArrayLength;
    _IsLeftValue = Value._IsLeftValue;
    _IsVariant = Value._IsVariant;
    _IsArray = Value._IsArray;

    return *this;
}

TParaValue::~TParaValue()
{
    if (_Type == vtString) {
	delete _PrimitiveValue._StringValue;
    }
    else if (_Type == vtList) {
	delete _PrimitiveValue._ListValue;
    }
}

void TParaValue::Destroy(void)
{
    //cout << "destroying " << TypeName() << ": " << AsString() << endl;

    if (_Type == vtObject) {
	this->AsObject()->Destruct();
        delete _PrimitiveValue._ObjectValue;
    }

    else if (_IsArray) {
	TParaValue* Element = this->AsPointer();
	int Length = Element->ArrayLength();
	for (int i = 0; i < Length; i++) {
	    Element[i].Destroy();
	}
    }
}

void TParaValue::SetName(string Name)
{
     if (IsObject()) {
	 _PrimitiveValue._ObjectValue->SetObjectName(Name);
     }
}

void TParaValue::Assign(const TParaValue& Value) throw(TScriptException)
{
    if (_IsVariant) {
	bool IsLeftValue = _IsLeftValue;
	int ArrayLength = _ArrayLength;

	this->operator=(Value);

	_IsVariant = true;
	_IsLeftValue = IsLeftValue;
	_ArrayLength = ArrayLength;

	if (Value.IsList() && _IsLeftValue) {
	    SetLeftValueFlag();
	}

	return;
    }

    else if (_Type == vtVoid) {
	;
    }
    else if (_Type == vtBool) {
        _PrimitiveValue._LongValue = Value.AsBool();
    }
    else if (_Type == vtLong) {
        _PrimitiveValue._LongValue = Value.AsLong();
    }
    else if (_Type == vtDouble) {
        _PrimitiveValue._DoubleValue = Value.AsDouble();
    }
    else if (_Type == vtString) {
        *_PrimitiveValue._StringValue = Value.AsString();
    }
    else if (_Type == vtObject) {
        throw TScriptException(
	    "TParaValue::Assign()", "assigning object to object is not allowed"
	);
    }
    else if (_Type == vtPointer) {
        _PrimitiveValue._PointerValue = Value.AsPointer();
    }
    else if (_Type == vtList) {
	*_PrimitiveValue._ListValue = Value.AsConstList();
    }
    else {
        throw TScriptException(
	    "TParaValue::Assign()", "unknown value type (internal error)"
	);
    }
}

bool TParaValue::IsVoid(void) const
{
    return (_Type == vtVoid);
}

bool TParaValue::IsBool(void) const
{
    return (_Type == vtBool);
}

bool TParaValue::IsLong(void) const
{
    return ((_Type == vtLong) || (_Type == vtVoid));
}

bool TParaValue::IsDouble(void) const
{
    return (_Type == vtDouble);
}

bool TParaValue::IsString(void) const
{
    return (_Type == vtString);
}

bool TParaValue::IsObject(void) const
{
    return (_Type == vtObject);
}

bool TParaValue::IsPointer(void) const
{
    return (_Type == vtPointer);
}

bool TParaValue::IsList(void) const
{
    return (_Type == vtList);
}

bool TParaValue::IsNumeric(void) const
{
    return (IsLong() || IsDouble() || IsVoid());
}

bool TParaValue::IsObject(const string& InternalClassName) const
{
    return (
	(_Type == vtObject) &&
	(_PrimitiveValue._ObjectValue->InternalClassName() == InternalClassName)
    );
}

bool TParaValue::AsBool(void) const throw(TScriptException)
{
    bool Result;

    switch (_Type) {
      case vtVoid:
	Result = false;
        break;
      case vtBool:
	Result = _PrimitiveValue._BoolValue;
        break;
      case vtLong:
	Result = (AsLong() != 0);
        break;
      case vtDouble:
	Result = (AsDouble() != 0);
        break;
      case vtString:
	Result = (! AsString().empty());
        break;
      case vtObject:
	Result = true;
        break;
      case vtPointer:
	Result = (AsPointer() != 0);
        break;
      case vtList:
	Result = (AsConstList().ListSize() > 0);
	break;
      default:
	throw TScriptException("unknown value type (internal)");
    }
    
    return Result;
}

long TParaValue::AsLong(void) const throw(TScriptException)
{
    long Value;
    string Remaining;
    bool IsConversionSuccessful = false;
    
    switch (_Type) {
      case vtVoid:
	Value = 0;
        IsConversionSuccessful = true;
        break;
      case vtBool:
        Value = (_PrimitiveValue._BoolValue ? 1 : 0);
        IsConversionSuccessful = true;
        break;
      case vtLong:
        Value = _PrimitiveValue._LongValue;
        IsConversionSuccessful = true;
        break;
      case vtDouble:
        Value = (long) _PrimitiveValue._DoubleValue;
        IsConversionSuccessful = true;
        break;
      case vtString:
	if (
	    (_PrimitiveValue._StringValue->size() > 2) && 
	    (tolower((*_PrimitiveValue._StringValue)[1]) == 'x')
	){
	    // Hex Number
	    istringstream ValueStream(_PrimitiveValue._StringValue->substr(2, string::npos));
	    if (ValueStream >> hex >> Value) {
		getline(ValueStream, Remaining);
		IsConversionSuccessful = Remaining.empty();
	    }
	}
        else {
	    // Dec Number
	    istringstream ValueStream(*(_PrimitiveValue._StringValue));
	    if (ValueStream >> Value) {
		getline(ValueStream, Remaining);
		IsConversionSuccessful = Remaining.empty();
	    }
        }
        break;
      case vtObject:
        break;
      case vtPointer:
        Value = (long) _PrimitiveValue._PointerValue;
        IsConversionSuccessful = true;
        break;
      case vtList:
        Value = (long) _PrimitiveValue._ListValue->ValueList().size();
        IsConversionSuccessful = true;
        break;
      default:
	throw TScriptException("unknown value type (internal)");
    }

    if (! IsConversionSuccessful) {	
        throw TScriptException(
	    "integer value is expected: value = \"" + AsString() + "\""
	);
    }
    
    return Value;
}

double TParaValue::AsDouble(void) const throw(TScriptException)
{
    double Value;
    string Remaining;
    bool IsConversionSuccessful = false;
    
    switch (_Type) {
      case vtVoid:
	Value = 0;
        IsConversionSuccessful = true;
        break;
      case vtBool:
        Value = (_PrimitiveValue._BoolValue ? 1.0 : 0.0);
        IsConversionSuccessful = true;
        break;
      case vtLong:
        Value = (double) _PrimitiveValue._LongValue;
        IsConversionSuccessful = true;
        break;
      case vtDouble:
        Value = _PrimitiveValue._DoubleValue;
        IsConversionSuccessful = true;
        break;
      case vtString:
        {
	    istringstream ValueStream(*(_PrimitiveValue._StringValue));
	    if (ValueStream >> Value) {
		getline(ValueStream, Remaining);
		IsConversionSuccessful = Remaining.empty();
	    }
	}
        break;
      case vtObject:
        break;
      case vtPointer:
        break;
      case vtList:
        break;
      default:
	throw TScriptException("unknown value type (internal)");
    }

    if (! IsConversionSuccessful) {
        throw TScriptException(
	    "floating value is expected: value = \"" + AsString() + "\""
	);
    }
    
    return Value;
}

string TParaValue::AsString(void) const throw(TScriptException)
{
    ostringstream BufferStream;    
    string Value;
    
    switch (_Type) {
      case vtVoid:
	Value = "";
        break;
      case vtBool:
        Value = (_PrimitiveValue._BoolValue ? "1" : "");
        break;
      case vtLong:
	BufferStream << _PrimitiveValue._LongValue;
        Value = BufferStream.str();
        break;
      case vtDouble:
        BufferStream << _PrimitiveValue._DoubleValue;
        Value = BufferStream.str();
        break;
      case vtString:
        Value = *_PrimitiveValue._StringValue;
        break;
      case vtObject:
	BufferStream << AsObject()->InternalClassName();
        BufferStream << "@" << AsObject();
        Value = BufferStream.str();
        break;
      case vtPointer:
        BufferStream << _PrimitiveValue._PointerValue;
        Value = BufferStream.str();
        break;
      case vtList:
	Value = _PrimitiveValue._ListValue->AsString();
	break;
      default:
	throw TScriptException("unknown value type (internal)");
    }
    
    return Value;
}

string& TParaValue::AsStringReference(void) const throw(TScriptException)
{
    if (_Type != vtString) {
        throw TScriptException("string value is expected");
    }

    return *_PrimitiveValue._StringValue;
}

TParaObjectPrototype* TParaValue::AsObject(void) const throw(TScriptException)
{
    if (_Type != vtObject) {
        throw TScriptException("object value is expected");
    }
    
    return _PrimitiveValue._ObjectValue;
}

TParaValue* TParaValue::AsPointer(void) const throw(TScriptException)
{
    if ((_Type == vtLong) && (AsLong() == 0)) {
        return (TParaValue*) _PrimitiveValue._LongValue;
    }
    else if (_Type != vtPointer) {
        throw TScriptException("pointer value is expected");
    }
    
    return _PrimitiveValue._PointerValue;
}

TParaListValue& TParaValue::AsList(void) throw(TScriptException)
{
    if (_Type != vtList) {
        throw TScriptException("list value is expected");
    }

    return *_PrimitiveValue._ListValue;
}

const TParaListValue& TParaValue::AsConstList(void) const throw(TScriptException)
{
    if (_Type != vtList) {
        throw TScriptException("list value is expected");
    }

    return *_PrimitiveValue._ListValue;
}

vector<TParaValue>& TParaValue::AsValueList(void) throw(TScriptException)
{
    if (_Type != vtList) {
        throw TScriptException("list value is expected");
    }

    return _PrimitiveValue._ListValue->ValueList();
}

void TParaValue::SetLeftValueFlag(void)
{
    _IsLeftValue = true;

    if (_Type == vtList) {
	vector<TParaValue>& ValueList = _PrimitiveValue._ListValue->ValueList();
	for (unsigned i = 0; i < ValueList.size(); i++) {
	    ValueList[i].SetLeftValueFlag();
	}
    }
}

bool TParaValue::IsLeftValue(void) const
{
    return _IsLeftValue;
}

void TParaValue::SetVariantFlag(void)
{
    _IsVariant = true;
}

bool TParaValue::IsVariant(void) const
{
    return _IsVariant;
}

void TParaValue::SetArrayFlag(void)
{
    _IsArray = true;
}

bool TParaValue::IsArray(void) const
{
    return _IsArray;
}

void TParaValue::SetArrayLength(int ArrayLength)
{
    _ArrayLength = ArrayLength;
}

int TParaValue::ArrayLength(void) const
{
    return _ArrayLength;
}

string TParaValue::TypeName(void) const
{
    string Value;
    switch (_Type) {
      case vtBool:
	Value = "bool";
        break;
      case vtLong:
	Value = "int";
        break;
      case vtDouble:
	Value = "float";
        break;
      case vtString:
	Value = "string";
        break;
      case vtObject:
        Value = AsObject()->InternalClassName();
        break;
      case vtPointer:
	Value = "pointer";
        break;
      case vtList:
	Value = "list";
	break;
      case vtVoid:
	Value = "void";
	break;
      default:
	throw TScriptException("unknown value type (internal)");
    }
    
    return Value;
}

void TParaValue::Dump(ostream& os) const
{
    os << TypeName();
    if (_IsVariant) {
	os << "{variant}";
    }
    if (_IsLeftValue) {
	os << "{left}";
    }
    if (_IsArray) {
	os << "[" << _ArrayLength << "]";
    }
    os << " " << AsString();
}



TParaListValue::TParaListValue(void)
{
    _KeyList = 0;
    _KeyIndexTable = 0;
    _IndexKeyTable = 0;
}

TParaListValue::TParaListValue(unsigned InitialCapacity)
{
    _ValueList.reserve(InitialCapacity);

    _KeyList = 0;
    _KeyIndexTable = 0;
    _IndexKeyTable = 0;
}

TParaListValue::TParaListValue(const vector<TParaValue>& ValueList)
: _ValueList(ValueList)
{
    _KeyList = 0;
    _KeyIndexTable = 0;
    _IndexKeyTable = 0;
}

TParaListValue::TParaListValue(const TParaListValue& ListValue)
{
    _ValueList = ListValue._ValueList;

    if (ListValue._KeyList) {
	_KeyList = new vector<string>(*ListValue._KeyList);
	_KeyIndexTable = new map<string, unsigned>(*ListValue._KeyIndexTable);
	_IndexKeyTable = new map<unsigned, string>(*ListValue._IndexKeyTable);
    }
    else {
	_KeyList = 0;
	_KeyIndexTable = 0;
	_IndexKeyTable = 0;
    }
}

TParaListValue& TParaListValue::operator=(const TParaListValue& ListValue)
{
    delete _KeyIndexTable;
    delete _KeyList;

    _ValueList = ListValue._ValueList;
    
    if (ListValue._KeyList) {
	_KeyList = new vector<string>(*ListValue._KeyList);
	_KeyIndexTable = new map<string, unsigned>(*ListValue._KeyIndexTable);
	_IndexKeyTable = new map<unsigned, string>(*ListValue._IndexKeyTable);
    }
    else {
	_KeyList = 0;
	_KeyIndexTable = 0;
	_IndexKeyTable = 0;
    }

    return *this;
}

TParaListValue::~TParaListValue()
{
    delete _IndexKeyTable;
    delete _KeyIndexTable;
    delete _KeyList;
}

unsigned TParaListValue::ListSize(void) const
{
    return _ValueList.size();
}

bool TParaListValue::HasKeyIndex(void) const
{
    return (_KeyList != 0);
}

void TParaListValue::AppendList(const TParaListValue& ListValue)
{
    unsigned StartIndex = _ValueList.size();
    _ValueList.insert(
	_ValueList.end(), 
	ListValue.ConstValueList().begin(), ListValue.ConstValueList().end()
    );

    if (ListValue.HasKeyIndex()) {
	const vector<string>& KeyList = ListValue.KeyList();
	for (unsigned i = 0; i < KeyList.size(); i++) {
	    SetKey(StartIndex + i, KeyList[i]);
	}
    }
}

unsigned TParaListValue::AppendValue(const TParaValue& Value)
{
    unsigned Index = _ValueList.size();

    _ValueList.push_back(Value);
    _ValueList[Index].SetLeftValueFlag();
    _ValueList[Index].SetVariantFlag();

    return Index;
}

vector<TParaValue>& TParaListValue::ValueList(void)
{
    return _ValueList;
}

const vector<TParaValue>& TParaListValue::ConstValueList(void) const
{
    return _ValueList;
}

const vector<string>& TParaListValue::KeyList(void) const
{
    if (_KeyList == 0) {
	_KeyList = new vector<string>();
	_KeyIndexTable = new map<string, unsigned>();
	_IndexKeyTable = new map<unsigned, string>();
    }

    return *_KeyList;
}

void TParaListValue::SetKey(unsigned Index, const string& Key)
{
    if (_KeyList == 0) {
	KeyList();
    }

    if (_KeyIndexTable->count(Key) == 0) {
	_KeyList->push_back(Key);
    }
    (*_KeyIndexTable)[Key] = Index;
    (*_IndexKeyTable)[Index] = Key;
}

string TParaListValue::KeyOf(unsigned Index)
{
    if (_IndexKeyTable && _IndexKeyTable->count(Index) > 0) {
	return (*_IndexKeyTable)[Index];
    }
    else {
	return "";
    }
}

unsigned TParaListValue::IndexOf(const string& Key)
{
    if (_KeyIndexTable->count(Key) == 0) {
	ValueOf(Key);
    }

    return (*_KeyIndexTable)[Key];
}

TParaValue& TParaListValue::ValueOf(unsigned Index)
{
    if (Index > _ValueList.size()) {
	unsigned OldSize = _ValueList.size();
	TParaVariant Variant;
	_ValueList.resize(Index + 1, TParaValue(Variant));

	for (unsigned i = OldSize; i < _ValueList.size(); i++) {
	    _ValueList[i].SetLeftValueFlag();
	}
    }

    return _ValueList[Index];
}

TParaValue& TParaListValue::ValueOf(const string& Key)
{
    if (_KeyList == 0) {
	KeyList();
    }

    unsigned Index;
    map<string, unsigned>::iterator Iterator = _KeyIndexTable->find(Key);
    if (Iterator != _KeyIndexTable->end()) {
	Index = (*Iterator).second;
    }
    else {
	TParaVariant Variant;
	TParaValue Value(Variant);
	Value.SetLeftValueFlag();

	Index = _ValueList.size();
	_ValueList.push_back(Value);

	SetKey(Index, Key);
    }

    return _ValueList[Index];
}

string TParaListValue::AsString(void) const
{
    string StringValue;

    StringValue = "{ ";
    for (unsigned i = 0; i < _ValueList.size(); i++) {
	if (_IndexKeyTable) {
	    string Key = (*_IndexKeyTable)[i];
	    if (! Key.empty()) {
		StringValue += "\"" + Key + "\" => ";
	    }
	}

	if (_ValueList[i].IsString()) {
	    StringValue += "\"" + _ValueList[i].AsString() + "\"";
	}
	else {
	    StringValue += _ValueList[i].AsString();
	}

	if (i < _ValueList.size() - 1) {
	    StringValue += ", ";
	}
    }
    StringValue += " }";

    return StringValue;
}
