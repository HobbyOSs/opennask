/* ParaStandardLibrary.cc */
/* Created by Enomoto Sanshiro on 22 August 1999. */
/* Last updated by Enomoto Sanshiro on 24 March 2002. */


#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <utility>
#include "ParaObject.hh"
#include "ParaStandardLibrary.hh"

using namespace std;


TParaConsoleObject::TParaConsoleObject(void)
: TParaObjectPrototype("Console")
{
}

TParaConsoleObject::~TParaConsoleObject()
{
}

TParaObjectPrototype* TParaConsoleObject::Clone(void)
{
    return new TParaConsoleObject();
}

int TParaConsoleObject::MethodIdOf(const string& MethodName)
{
    if (MethodName == "print") {
        return MethodId_Print;
    }
    else if ((MethodName == "printLine") || (MethodName == "println")) {
        return MethodId_PrintLine;
    }
    else if ((MethodName == "getLine") || (MethodName == "getln")) {
        return MethodId_GetLine;
    }
    else if ((MethodName == "getByte") || (MethodName == "getc")) {
        return MethodId_GetByte;
    }

    return TParaObjectPrototype::MethodIdOf(MethodName);
}

int TParaConsoleObject::InvokeMethod(int MethodId, vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    int Result = 0;

    switch (MethodId) {
      case MethodId_Print:
        Result = Print(ArgumentList, ReturnValue);
	break;
      case MethodId_PrintLine:
        Result = PrintLine(ArgumentList, ReturnValue);
	break;
      case MethodId_GetLine:
        Result = GetLine(ArgumentList, ReturnValue);
	break;
      case MethodId_GetByte:
        Result = GetByte(ArgumentList, ReturnValue);
	break;
      default:
	Result = 0;
    }

    return Result;
}

int TParaConsoleObject::Print(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    for (unsigned i = 0; i < ArgumentList.size(); i++) {
        cout << ArgumentList[i]->AsString();
    }
    cout << flush;

    return 1;
}

int TParaConsoleObject::PrintLine(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    Print(ArgumentList, ReturnValue);
    cout << endl;

    return 1;
}

int TParaConsoleObject::GetLine(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    char LineTerminator = '\n';
    if (ArgumentList.size() > 0) {
	LineTerminator = ArgumentList[0]->AsString()[0];
    }

    string Line;
    if (getline(cin, Line, LineTerminator)) {
	ReturnValue = TParaValue(Line + LineTerminator);
    }
    else {
	ReturnValue = TParaValue(string(""));
    }

    return 1;
}

int TParaConsoleObject::GetByte(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    long Value = -1;

    char ch;
    if (cin.get(ch)) {
	Value = (unsigned char) ch;
    }

    ReturnValue = TParaValue(Value);

    return 1;
}



TParaInputFileObject::TParaInputFileObject(void)
: TParaObjectPrototype("InputFile")
{
    _FileStream = 0;
}

TParaInputFileObject::~TParaInputFileObject()
{
    delete _FileStream;
}

TParaObjectPrototype* TParaInputFileObject::Clone(void)
{
    return new TParaInputFileObject();
}

void TParaInputFileObject::Construct(const string& ClassName, vector<TParaValue*>& ArgumentList) throw(TScriptException)
{
    if (ArgumentList.size() < 1) {
	throw TScriptException(
	    _InternalClassName + "::" + _InternalClassName + "()",
	    "too few argument[s]"
	);
    }
    if (! ArgumentList[0]->IsString()) {
	throw TScriptException(
	    _InternalClassName + "::" + _InternalClassName + "()",
	    "invalid argument[s]"
	);
    }

    string FileName = ArgumentList[0]->AsString();
    _FileStream = new ifstream(FileName.c_str());
    if (! *_FileStream) {
	throw TScriptException(
	    _InternalClassName + "::" + _InternalClassName + "()",
	    "unable to open file: " + FileName
	);
    }
}

int TParaInputFileObject::MethodIdOf(const string& MethodName)
{
    if ((MethodName == "getLine") || (MethodName == "getln")) {
        return MethodId_GetLine;
    }
    else if ((MethodName == "getByte") || (MethodName == "getc")) {
        return MethodId_GetByte;
    }

    return TParaObjectPrototype::MethodIdOf(MethodName);
}

int TParaInputFileObject::InvokeMethod(int MethodId, vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    int Result = 0;

    switch (MethodId) {
      case MethodId_GetLine:
        Result = GetLine(ArgumentList, ReturnValue);
	break;
      case MethodId_GetByte:
        Result = GetByte(ArgumentList, ReturnValue);
	break;
      default:
	Result = 0;
    }

    return Result;
}

int TParaInputFileObject::GetLine(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    char LineTerminator = '\n';
    if (ArgumentList.size() > 0) {
	LineTerminator = ArgumentList[0]->AsString()[0];
    }

    string Line;
    if (getline(*_FileStream, Line, LineTerminator)) {
	ReturnValue = TParaValue(Line + LineTerminator);
    }
    else {
	ReturnValue = TParaValue(string(""));
    }

    return 1;
}

int TParaInputFileObject::GetByte(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    long Value = -1;

    char ch;
    if (_FileStream->get(ch)) {
	Value = (unsigned char) ch;
    }

    ReturnValue = TParaValue(Value);

    return 1;
}



TParaOutputFileObject::TParaOutputFileObject(void)
: TParaObjectPrototype("OutputFile")
{
    _FileStream = 0;
}

TParaOutputFileObject::~TParaOutputFileObject()
{
    delete _FileStream;
}

TParaObjectPrototype* TParaOutputFileObject::Clone(void)
{
    return new TParaOutputFileObject();
}

void TParaOutputFileObject::Construct(const string& ClassName, vector<TParaValue*>& ArgumentList) throw(TScriptException)
{
    if (ArgumentList.size() < 1) {
	throw TScriptException(
	    _InternalClassName + "::" + _InternalClassName + "()",
	    "too few argument[s]"
	);
    }
    if (! ArgumentList[0]->IsString()) {
	throw TScriptException(
	    _InternalClassName + "::" + _InternalClassName + "()",
	    "invalid argument[s]"
	);
    }

    string FileName = ArgumentList[0]->AsString();
    _FileStream = new ofstream(FileName.c_str());
    if (! *_FileStream) {
	throw TScriptException(
	    _InternalClassName + "::" + _InternalClassName + "()",
	    "unable to open file: " + FileName
	);
    }
}

int TParaOutputFileObject::MethodIdOf(const string& MethodName)
{
    if (MethodName == "print") {
        return MethodId_Print;
    }
    else if ((MethodName == "printLine") || (MethodName == "println")) {
        return MethodId_PrintLine;
    }

    return TParaObjectPrototype::MethodIdOf(MethodName);
}

int TParaOutputFileObject::InvokeMethod(int MethodId, vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    int Result = 0;

    switch (MethodId) {
      case MethodId_Print:
	Result = Print(ArgumentList, ReturnValue);
	break;
      case MethodId_PrintLine:
        Result = PrintLine(ArgumentList, ReturnValue);
	break;
      default:
	Result = 0;
    }

    return Result;
}

int TParaOutputFileObject::Print(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    for (unsigned i = 0; i < ArgumentList.size(); i++) {
        (*_FileStream) << ArgumentList[i]->AsString();
    }
    (*_FileStream) << flush;

    return 1;
}

int TParaOutputFileObject::PrintLine(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    Print(ArgumentList, ReturnValue);
    (*_FileStream) << endl;

    return 1;
}



TParaInputPipeObject::TParaInputPipeObject(void)
: TParaInputFileObject()
{
    _Pipe = 0;
}

TParaInputPipeObject::~TParaInputPipeObject()
{
    if ((_Pipe != 0) && (_Pipe != NULL)) {
	pclose(_Pipe);
    }
}

TParaObjectPrototype* TParaInputPipeObject::Clone(void)
{
    return new TParaInputPipeObject();
}

void TParaInputPipeObject::Construct(const string& ClassName, vector<TParaValue*>& ArgumentList) throw(TScriptException)
{
    if (ArgumentList.size() < 1) {
	throw TScriptException(
	    "InputPipe::InputPipe(string command)", "too few argument[s]"
	);
    }
    if (! ArgumentList[0]->IsString()) {
	throw TScriptException(
	    "InputPipe::InputPipe(string command)", "too few argument[s]"
	);
    }

    string CommandName = ArgumentList[0]->AsString();
    _Pipe = popen(CommandName.c_str(), "r");
    if ((_Pipe == 0) || (_Pipe == NULL)) {
	throw TScriptException(
	    "InputPipe::InputPipe(string command)", "too few argument[s]"
	    "unable to open pipe: '" + CommandName + "'"
	);
    }
}

int TParaInputPipeObject::GetLine(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    char LineTerminator = '\n';
    if (ArgumentList.size() > 0) {
	LineTerminator = ArgumentList[0]->AsString()[0];
    }

    ReturnValue = TParaValue(string());
    string& Line = ReturnValue.AsStringReference();

    int ch;
    while ((ch = fgetc(_Pipe)) != EOF) {
	Line += ch;
	if (ch == LineTerminator) {
	    break;
	}
    }

    return 1;
}

int TParaInputPipeObject::GetByte(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    long Value = -1;

    int ch = fgetc(_Pipe);
    if (ch != EOF) {
	Value = (unsigned char) ch;
    }

    ReturnValue = TParaValue(Value);

    return 1;
}



TParaOutputPipeObject::TParaOutputPipeObject(void)
: TParaOutputFileObject()
{
    _Pipe = 0;
}

TParaOutputPipeObject::~TParaOutputPipeObject()
{
    if ((_Pipe != 0) && (_Pipe != NULL)) {
	pclose(_Pipe);
    }
}

TParaObjectPrototype* TParaOutputPipeObject::Clone(void)
{
    return new TParaOutputPipeObject();
}

void TParaOutputPipeObject::Construct(const string& ClassName, vector<TParaValue*>& ArgumentList) throw(TScriptException)
{
    if (ArgumentList.size() < 1) {
	throw TScriptException(
	    "OutputPipe::OutputPipe(string command)", "too few argument[s]"
	);
    }
    if (! ArgumentList[0]->IsString()) {
	throw TScriptException(
	    "OutputPipe::OutputPipe(string command)", "too few argument[s]"
	);
    }

    string CommandName = ArgumentList[0]->AsString();
    _Pipe = popen(CommandName.c_str(), "w");
    if ((_Pipe == 0) || (_Pipe == NULL)) {
	throw TScriptException(
	    "InputPipe::InputPipe(string command)", "too few argument[s]"
	    "unable to open pipe: '" + CommandName + "'"
	);
    }
}

int TParaOutputPipeObject::Print(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    for (unsigned i = 0; i < ArgumentList.size(); i++) {
        fputs(ArgumentList[i]->AsString().c_str(), _Pipe);
    }
    fflush(_Pipe);

    return 1;
}

int TParaOutputPipeObject::PrintLine(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    Print(ArgumentList, ReturnValue);
    fputc('\n', _Pipe);

    return 1;
}



TParaFormatterObject::TParaFormatterObject(void)
: TParaObjectPrototype("Formatter")
{
    _FormatStream = new ostringstream();
}

TParaFormatterObject::~TParaFormatterObject()
{
    delete _FormatStream;
}

TParaObjectPrototype* TParaFormatterObject::Clone(void)
{
    return new TParaFormatterObject();
}

int TParaFormatterObject::MethodIdOf(const std::string& MethodName)
{
    if (MethodName == "put") {
        return MethodId_Put;
    }
    else if (MethodName == "flush") {
        return MethodId_Flush;
    }
    else if ((MethodName == "setWidth") || (MethodName == "setw")) {
        return MethodId_SetWidth;
    }
    else if ((MethodName == "setPrecision") || (MethodName == "setprecision")) {
        return MethodId_SetPrecision;
    }
    else if ((MethodName == "setFill") || (MethodName == "setfill")) {
        return MethodId_SetFill;
    }
    else if ((MethodName == "setBase") || (MethodName == "setbase")) {
        return MethodId_SetBase;
    }
    else if (MethodName == "hex") {
        return MethodId_Hex;
    }
    else if (MethodName == "dec") {
        return MethodId_Dec;
    }
    else if (MethodName == "fixed") {
        return MethodId_Fixed;
    }
    else if (MethodName == "scientific") {
        return MethodId_Scientific;
    }

    return TParaObjectPrototype::MethodIdOf(MethodName);
}

int TParaFormatterObject::InvokeMethod(int MethodId, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    int Result = 0;

    switch (MethodId) {
      case MethodId_Put:
        Result = Put(ArgumentList, ReturnValue);
	break;
      case MethodId_Flush:
        Result = Flush(ArgumentList, ReturnValue);
	break;
      case MethodId_SetWidth:
        Result = SetWidth(ArgumentList, ReturnValue);
	break;
      case MethodId_SetPrecision:
        Result = SetPrecision(ArgumentList, ReturnValue);
	break;
      case MethodId_SetFill:
        Result = SetFill(ArgumentList, ReturnValue);
	break;
      case MethodId_SetBase:
        Result = SetBase(ArgumentList, ReturnValue);
	break;
      case MethodId_Hex:
        Result = Hex(ArgumentList, ReturnValue);
	break;
      case MethodId_Dec:
        Result = Dec(ArgumentList, ReturnValue);
	break;
      case MethodId_Fixed:
        Result = Fixed(ArgumentList, ReturnValue);
	break;
      case MethodId_Scientific:
        Result = Scientific(ArgumentList, ReturnValue);
	break;
      default:
	break;
    }

    return Result;
}

int TParaFormatterObject::Flush(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    string Value = _FormatStream->str();

    delete _FormatStream;
    _FormatStream = new ostringstream();

    ReturnValue = TParaValue(Value);

    return 1;
}

int TParaFormatterObject::Put(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    for (unsigned i = 0; i < ArgumentList.size(); i++) {
	if (ArgumentList[i]->IsLong()) {
	    (*_FormatStream) << ArgumentList[i]->AsLong();
	}
	else if (ArgumentList[i]->IsDouble()) {
	    (*_FormatStream) << ArgumentList[i]->AsDouble();
	}
	else {
	    (*_FormatStream) << ArgumentList[i]->AsString();
	}
    }

    ReturnValue = TParaValue((TParaObjectPrototype*) this);

    return 1;
}

int TParaFormatterObject::SetWidth(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (ArgumentList.size() != 1) {
        throw TScriptException("setWidth(): incorrect argument");
    }

    int Width = ArgumentList[0]->AsLong();
    (*_FormatStream) << setw(Width);

    ReturnValue = TParaValue((TParaObjectPrototype*) this);

    return 1;
}

int TParaFormatterObject::SetPrecision(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (ArgumentList.size() != 1) {
        throw TScriptException("setPrecision(): incorrect argument");
    }

    int Precision = ArgumentList[0]->AsLong();
    (*_FormatStream) << setprecision(Precision);

    ReturnValue = TParaValue((TParaObjectPrototype*) this);

    return 1;
}

int TParaFormatterObject::SetFill(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (ArgumentList.size() != 1) {
        throw TScriptException("setFill(): incorrect argument");
    }

    char Fill = ArgumentList[0]->AsString()[0];
    (*_FormatStream) << setfill(Fill);

    ReturnValue = TParaValue((TParaObjectPrototype*) this);

    return 1;
}

int TParaFormatterObject::SetBase(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (ArgumentList.size() != 1) {
        throw TScriptException("setBase(): incorrect argument");
    }

    int Base = ArgumentList[0]->AsLong();
    if (Base == 10) {
	(*_FormatStream) << dec;
    }
    else if (Base == 16) {
	(*_FormatStream) << hex;
    }
    else {
	(*_FormatStream) << setbase(Base);
    }

    ReturnValue = TParaValue((TParaObjectPrototype*) this);

    return 1;
}

int TParaFormatterObject::Hex(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    (*_FormatStream) << hex;

    ReturnValue = TParaValue((TParaObjectPrototype*) this);

    return 1;
}

int TParaFormatterObject::Dec(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    (*_FormatStream) << dec;

    ReturnValue = TParaValue((TParaObjectPrototype*) this);

    return 1;
}

int TParaFormatterObject::Fixed(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    _FormatStream->setf(ios::fixed);

    ReturnValue = TParaValue((TParaObjectPrototype*) this);

    return 1;
}

int TParaFormatterObject::Scientific(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    _FormatStream->setf(ios::scientific);

    ReturnValue = TParaValue((TParaObjectPrototype*) this);

    return 1;
}



TParaScannerObject::TParaScannerObject(void)
: TParaObjectPrototype("Scanner")
{
    _SourceStream = 0;
}

TParaScannerObject::~TParaScannerObject()
{
    delete _SourceStream;
}

TParaObjectPrototype* TParaScannerObject::Clone(void)
{
    return new TParaScannerObject();
}

void TParaScannerObject::Construct(const string& ClassName, vector<TParaValue*>& ArgumentList) throw(TScriptException)
{
    if (ArgumentList.size() > 0) {
	if (! ArgumentList[0]->IsString()) {
	    throw TScriptException(
		_InternalClassName + "::" + _InternalClassName + "()",
		"invalid argumrnt[s]"
	    );
	}

	TParaValue ReturnValue;
	Load(ArgumentList, ReturnValue);
    }
}

int TParaScannerObject::MethodIdOf(const string& MethodName)
{
    if (MethodName == "load") {
        return MethodId_Load;
    }
    else if (MethodName == "get") {
        return MethodId_Get;
    }
    else if (MethodName == "getLine") {
        return MethodId_GetLine;
    }
    else if ((MethodName == "skipWhiteSpace") || (MethodName == "skipws")) {
        return MethodId_SkipWhiteSpace;
    }
    else if ((MethodName == "setBase") || (MethodName == "setbase")) {
        return MethodId_SetBase;
    }
    else if ((MethodName == "isGood") || (MethodName == "good")) {
        return MethodId_IsGood;
    }
    else if ((MethodName == "LastGetCount") || (MethodName == "gcount")) {
        return MethodId_LastGetCount;
    }

    return TParaObjectPrototype::MethodIdOf(MethodName);
}

int TParaScannerObject::InvokeMethod(int MethodId, vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    int Result = 0;

    switch (MethodId) {
      case MethodId_Load:
        Result = Load(ArgumentList, ReturnValue);
	break;
      case MethodId_Get:
        Result = Get(ArgumentList, ReturnValue);
	break;
      case MethodId_GetLine:
        Result = GetLine(ArgumentList, ReturnValue);
	break;
      case MethodId_SkipWhiteSpace:
        Result = SkipWhiteSpace(ArgumentList, ReturnValue);
	break;
      case MethodId_SetBase:
        Result = SetBase(ArgumentList, ReturnValue);
	break;
      case MethodId_IsGood:
        Result = IsGood(ArgumentList, ReturnValue);
	break;
      case MethodId_LastGetCount:
        Result = LastGetCount(ArgumentList, ReturnValue);
	break;
      default:
	Result = 0;
    }

    return Result;
}

int TParaScannerObject::Load(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (ArgumentList.size() < 1) {
	throw TScriptException(
	    _InternalClassName + "::load()", "too few argumrnt[s]"
	);
    }
    if (! ArgumentList[0]->IsString()) {
	throw TScriptException(
	    _InternalClassName + "::load()", "invalid argumrnt[s]"
	);
    }

    delete _SourceStream;
    _SourceStream = new istringstream(ArgumentList[0]->AsString());

    ReturnValue = TParaValue((TParaObjectPrototype*) this);

    return 1;
}

int TParaScannerObject::Get(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (_SourceStream == 0) {
	throw TScriptException(
	    _InternalClassName + "::get()", "empty buffer"
	);
    }

    if ((ArgumentList.size() != 1) || (! ArgumentList[0]->IsLeftValue())) {
	throw TScriptException(
	    _InternalClassName + "::get(...)", "invalid argument"
	);
    }

    if (ArgumentList[0]->IsVariant() || ArgumentList[0]->IsString()) {
	string Value;
	(*_SourceStream) >> Value;
	ArgumentList[0]->Assign(TParaValue(Value));
    }
    else if (ArgumentList[0]->IsLong()) {
	long Value;
	(*_SourceStream) >> Value;
	ArgumentList[0]->Assign(TParaValue(Value));
    }
    else if (ArgumentList[0]->IsDouble()) {
	double Value;
	(*_SourceStream) >> Value;
	ArgumentList[0]->Assign(TParaValue(Value));
    }
    else {
	throw TScriptException(
	    _InternalClassName + "::get(...)", "invalid argument type"
	);
    }

    ReturnValue = TParaValue((TParaObjectPrototype*) this);

    return 1;
}

int TParaScannerObject::GetLine(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (_SourceStream == 0) {
	throw TScriptException(
	    _InternalClassName + "::getLine(string line)", "empty buffer"
	);
    }

    if ((ArgumentList.size() < 1) || (! ArgumentList[0]->IsLeftValue())) {
	throw TScriptException(
	    _InternalClassName + "::getLine(string line)", "invalid argument"
	);
    }

    char LineTerminator = '\n';
    if (ArgumentList.size() > 1) {
	LineTerminator = ArgumentList[1]->AsString()[0];
    }

    if (ArgumentList[0]->IsVariant() || ArgumentList[0]->IsString()) {
	string Value;
	getline((*_SourceStream), Value, LineTerminator);
	ArgumentList[0]->Assign(TParaValue(Value));
    }
    else {
	throw TScriptException(
	    _InternalClassName + "::get(string line)", "invalid argument type"
	);
    }

    ReturnValue = TParaValue((TParaObjectPrototype*) this);

    return 1;
}

int TParaScannerObject::SkipWhiteSpace(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    (*_SourceStream) >> ws;

    ReturnValue = TParaValue((TParaObjectPrototype*) this);

    return 1;
}

int TParaScannerObject::SetBase(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (ArgumentList.size() != 1) {
        throw TScriptException("setBase(): incorrect argument");
    }

    int Base = ArgumentList[0]->AsLong();
    if (Base == 10) {
	(*_SourceStream) >> dec;
    }
    else if (Base == 16) {
	(*_SourceStream) >> hex;
    }
    else {
	(*_SourceStream) >> setbase(Base);
    }

    ReturnValue = TParaValue((TParaObjectPrototype*) this);

    return 1;
}

int TParaScannerObject::IsGood(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    ReturnValue = TParaValue((long) _SourceStream->good());

    return 1;
}

int TParaScannerObject::LastGetCount(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    ReturnValue = TParaValue((long) _SourceStream->gcount());

    return 1;
}



TParaArgumentObject::TParaArgumentObject(int argc, char** argv)
: TParaObjectPrototype("Argument")
{
    _argc = argc;
    _argv = argv;

    _IsParsed = false;
}

TParaArgumentObject::~TParaArgumentObject()
{
}

TParaObjectPrototype* TParaArgumentObject::Clone(void)
{
    return new TParaArgumentObject(_argc, _argv);
}

int TParaArgumentObject::MethodIdOf(const string& MethodName)
{
    if (MethodName == "numberOfArguments") {
        return MethodId_NumberOfArguments;
    }
    else if (MethodName == "getArgumentOf") {
        return MethodId_GetArgumentOf;
    }
    else if (MethodName == "numberOfParameters") {
        return MethodId_NumberOfParameters;
    }
    else if (MethodName == "getParameterOf") {
        return MethodId_GetParameterOf;
    }
    else if (MethodName == "isOptionSpecified") {
        return MethodId_IsOptionSpecified;
    }
    else if (MethodName == "getOptionValueOf") {
        return MethodId_GetOptionValueOf;
    }
    else if (MethodName == "isSwitchSpecified") {
        return MethodId_IsSwitchSpecified;
    }

    return TParaObjectPrototype::MethodIdOf(MethodName);
}

int TParaArgumentObject::InvokeMethod(int MethodId, vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (! _IsParsed) {
	Parse();
    }

    int Result = 0;

    switch (MethodId) {
      case MethodId_NumberOfArguments:
        Result = NumberOfArguments(ArgumentList, ReturnValue);
	break;
      case MethodId_GetArgumentOf:
        Result = GetArgumentOf(ArgumentList, ReturnValue);
	break;
      case MethodId_NumberOfParameters:
        Result = NumberOfParameters(ArgumentList, ReturnValue);
	break;
      case MethodId_GetParameterOf:
        Result = GetParameterOf(ArgumentList, ReturnValue);
	break;
      case MethodId_IsOptionSpecified:
        Result = IsOptionSpecified(ArgumentList, ReturnValue);
	break;
      case MethodId_GetOptionValueOf:
        Result = GetOptionValueOf(ArgumentList, ReturnValue);
	break;
      case MethodId_IsSwitchSpecified:
        Result = IsSwitchSpecified(ArgumentList, ReturnValue);
	break;
      default:
	return TParaObjectPrototype::InvokeMethod(MethodId, ArgumentList, ReturnValue);
    }

    return Result;
}

int TParaArgumentObject::NumberOfArguments(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    ReturnValue = TParaValue((long) _ArgumentList.size());

    return 1;
}

int TParaArgumentObject::GetArgumentOf(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (ArgumentList.size() != 1) {
        throw TScriptException("getArgumentOf(int index): incorrect argument");
    }

    int Index = ArgumentList[0]->AsLong();
    string Argument;
    if (Index < (int) _ArgumentList.size()) {
	Argument = _ArgumentList[Index];
    }

    ReturnValue = TParaValue(Argument);

    return 1;
}

int TParaArgumentObject::IsOptionSpecified(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if ((ArgumentList.size() != 1) && (ArgumentList.size() != 2)) {
        throw TScriptException("isOptionSpecified(): incorrect argument");
    }

    string Name = ArgumentList[0]->AsString();
    long Result = (long) (_OptionTable.count(Name) > 0);

    if ((! Result) && (ArgumentList.size() > 1)) {
	string Switch = ArgumentList[1]->AsString();
	if (Switch.size() != 1) {
	    throw TScriptException("isOptionSpecified(): incorrect argument");
	}
	Result = (long) (_SwitchSet.count(Switch[0]) > 0);
    }

    ReturnValue = TParaValue(Result);

    return 1;
}

int TParaArgumentObject::GetOptionValueOf(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (ArgumentList.size() != 1) {
        throw TScriptException("getOptionValueOf(): incorrect argument");
    }

    string Name = ArgumentList[0]->AsString();
    string Value;
    if (_OptionTable.count(Name) > 0) {
	Value = _OptionTable[Name];
    }

    ReturnValue = TParaValue(Value);

    return 1;
}

int TParaArgumentObject::IsSwitchSpecified(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (ArgumentList.size() != 1) {
        throw TScriptException("isSwitchSpecified(): incorrect argument");
    }

    string Switch = ArgumentList[0]->AsString();
    if (Switch.size() != 1) {
        throw TScriptException("isSwitchSpecified(): incorrect argument");
    }

    ReturnValue = TParaValue((long) (_SwitchSet.count(Switch[0]) > 0));

    return 1;
}

int TParaArgumentObject::NumberOfParameters(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    ReturnValue = TParaValue((long) _ParameterList.size());

    return 1;
}

int TParaArgumentObject::GetParameterOf(vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    if (ArgumentList.size() != 1) {
        throw TScriptException("getParameterOf(int index): incorrect argument");
    }

    int Index = ArgumentList[0]->AsLong();
    string Parameter;
    if (Index < (int) _ParameterList.size()) {
	Parameter = _ParameterList[Index];
    }

    ReturnValue = TParaValue(Parameter);

    return 1;
}

void TParaArgumentObject::Parse(void)
{
    if (_argc > 0) {
	_ArgumentList.push_back(_argv[0]);
    }

    for (int i = 1; i < _argc; i++) {
	string Argument = _argv[i];
	_ArgumentList.push_back(Argument);

	if ((Argument[0] != '-') || (Argument == "-") || (Argument == "--")) {
	    _ParameterList.push_back(Argument);
	}
	else if ((Argument.size() > 1) && (isdigit(Argument[1]))) {
	    // negative number parameter //
	    _ParameterList.push_back(Argument);
	}
	else {
	    size_t NameLength = Argument.find_first_of('=');
	    string Name = Argument.substr(0, NameLength);
	    string Value = "";

	    if (NameLength != string::npos) {
		Value = Argument.substr(NameLength + 1, Argument.size());
	    }

	    if (Value.empty() && (i+1 < _argc)) {
		if (NameLength != string::npos) {
		    // already contains the assign operator//
		    i++;
		    Value = _argv[i];
		}
		else if (_argv[i+1][0] == '=') {
		    i++;
		    if (_argv[i][1] != '\0') {
			Value = (_argv[i] + 1);
		    }
		    else if (i+1 < _argc) {
			i++;
			Value = _argv[i];
		    }
		}
	    }

	    _OptionTable[Name] = Value;

	    if ((Argument.size() > 1) && (Argument[1] != '-')) {
		for (unsigned j = 1; j < Argument.size(); j++) {
		    _SwitchSet.insert(Argument[j]);
		}
	    }
	}
    }

    _IsParsed = true;
}



TParaStringObject::TParaStringObject(void)
: TParaObjectPrototype("String")
{
}

TParaStringObject::~TParaStringObject()
{
}

TParaObjectPrototype* TParaStringObject::Clone(void)
{
    return new TParaStringObject();
}

int TParaStringObject::MethodIdOf(const std::string& MethodName)
{
    if (MethodName == "chop") {
        return MethodId_Chop;
    }
    else if (MethodName == "chomp") {
        return MethodId_Chomp;
    }
    else if (MethodName == "substr") {
        return MethodId_Substr;
    }
    else if (MethodName == "index") {
        return MethodId_Index;
    }

    return TParaObjectPrototype::MethodIdOf(MethodName);
}

int TParaStringObject::InvokeMethod(int MethodId, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    int Result = 0;

    switch (MethodId) {
      case MethodId_Chop:
	Result = Chop(ArgumentList, ReturnValue);
	break;
      case MethodId_Chomp:
	Result = Chomp(ArgumentList, ReturnValue);
	break;
      case MethodId_Substr:
	Result = Substr(ArgumentList, ReturnValue);
	break;
      case MethodId_Index:
	Result = Index(ArgumentList, ReturnValue);
	break;
      default:
	Result = 0;
    }

    return Result;
}

int TParaStringObject::Chop(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) throw(TScriptException)
{
    if ((ArgumentList.size() != 1) || (! ArgumentList[0]->IsString())) {
	throw TScriptException(
	    "chop(string line)", "invalid argument[s]"
	);
    }

    string& Line = ArgumentList[0]->AsStringReference();
    string LastCharacter;
    if (! Line.empty()) {
	LastCharacter = Line[Line.size() - 1];
	Line.erase(Line.end() - 1);
    }

    Result = TParaValue(LastCharacter);

    return 1;
}

int TParaStringObject::Chomp(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) throw(TScriptException)
{
    if ((ArgumentList.size() != 1) || (! ArgumentList[0]->IsString())) {
	throw TScriptException(
	    "chomp(string line)", "invalid argument[s]"
	);
    }

    string& Line = ArgumentList[0]->AsStringReference();
    long NumberOfRemoved = 0;
    while ((! Line.empty()) && (*(Line.end()-1) == '\n')) {
	Line.erase(Line.end() - 1);
	NumberOfRemoved++;
    }

    Result = TParaValue(NumberOfRemoved);

    return 1;
}

int TParaStringObject::Substr(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) throw(TScriptException)
{
    if (
	(ArgumentList.size() < 2) ||
	(! ArgumentList[0]->IsString()) ||
	(! ArgumentList[1]->IsLong())
    ){
	throw TScriptException(
	    "substr(string line, int offset, int length=-1)",
	    "invalid argument[s]"
	);
    }
    string Line = ArgumentList[0]->AsString();

    int Offset = ArgumentList[1]->AsLong();
    if (Offset < 0) {
	Offset = Line.size() + Offset;
    }

    size_t Length;
    if (ArgumentList.size() > 2) {
	Length = ArgumentList[2]->AsLong();
	if (Length < 0) {
	    // perl compatible //
	    Length = max(0, static_cast<int>(Line.size() - Offset + Length));
	}
    }
    else {
	Length = string::npos;
    }

    Result = TParaValue(Line.substr(Offset, Length));

    return 1;
}

int TParaStringObject::Index(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) throw(TScriptException)
{
    if (
	(ArgumentList.size() < 2) ||
	(! ArgumentList[0]->IsString()) ||
	(! ArgumentList[1]->IsString())
    ){
	throw TScriptException(
	    "substr(string str, string sub_str, int offset=0)",
	    "invalid argument[s]"
	);
    }
    string Line = ArgumentList[0]->AsString();
    string Substring = ArgumentList[1]->AsString();

    int Offset;
    if (ArgumentList.size() > 2) {
	Offset = ArgumentList[2]->AsLong();
    }
    else {
	Offset = 0;
    }

    string::size_type Position = Line.find(Substring, Offset);

    Result = TParaValue((Position != string::npos) ? (long) Position : -1L);

    return 1;
}



TParaSystemObject::TParaSystemObject(void)
: TParaObjectPrototype("System")
{
}

TParaSystemObject::~TParaSystemObject()
{
}

TParaObjectPrototype* TParaSystemObject::Clone(void)
{
    return new TParaSystemObject();
}

int TParaSystemObject::MethodIdOf(const std::string& MethodName)
{
    if (MethodName == "system") {
        return MethodId_System;
    }
    else if (MethodName == "shell") {
        return MethodId_Shell;
    }

    return TParaObjectPrototype::MethodIdOf(MethodName);
}

int TParaSystemObject::InvokeMethod(int MethodId, std::vector<TParaValue*>& ArgumentList, TParaValue& ReturnValue) throw(TScriptException)
{
    int Result = 0;

    switch (MethodId) {
      case MethodId_System:
        Result = System(ArgumentList, ReturnValue);
	break;
      case MethodId_Shell:
        Result = Shell(ArgumentList, ReturnValue);
	break;
      default:
	Result = 0;
    }

    return Result;
}

int TParaSystemObject::System(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) throw(TScriptException)
{
    if ((ArgumentList.size() != 1) || (! ArgumentList[0]->IsString())) {
	throw TScriptException(
	    "system(string command)", "invalid argument[s]"
	);
    }

    string Command = ArgumentList[0]->AsString();
    long ReturnValue = system(Command.c_str());

    Result = TParaValue(ReturnValue);

    return 1;
}

int TParaSystemObject::Shell(std::vector<TParaValue*>& ArgumentList, TParaValue& Result) throw(TScriptException)
{
    if ((ArgumentList.size() != 1) || (! ArgumentList[0]->IsString())) {
	throw TScriptException(
	    "shell(string command)", "invalid argument[s]"
	);
    }

    string Command = ArgumentList[0]->AsString();

    FILE* Pipe = popen(Command.c_str(), "r");

    string ReturnValue;
    char Buffer[1024];
    while (fgets(Buffer, sizeof(Buffer), Pipe) != NULL) {
	ReturnValue += Buffer;
    }

    pclose(Pipe);

    Result = TParaValue(ReturnValue);

    return 1;
}
