/* MacroDrawParser.cc */
/* Created by Enomoto Sanshiro on 19 October 2001. */
/* Last updated by Enomoto Sanshiro on 19 October 2001. */


#include "Canvas.hh"
#include "CanvasMessenger.hh"
#include "ParaParser.hh"
#include "MacroDrawParser.hh"

using namespace std;


TMacroDrawParser::TMacroDrawParser(TCanvas* Canvas)
{
    _Canvas = Canvas;
}

TMacroDrawParser::~TMacroDrawParser()
{
}

TParaBuiltinFunctionTable* TMacroDrawParser::CreateBuiltinFunctionTable(void)
{
    TParaBuiltinFunctionTable* BuiltinFunctionTable;
    BuiltinFunctionTable = TParaStandardParser::CreateBuiltinFunctionTable();

    BuiltinFunctionTable->RegisterAnonymousClass(new TCanvasMessenger(_Canvas));

    return BuiltinFunctionTable;
}
