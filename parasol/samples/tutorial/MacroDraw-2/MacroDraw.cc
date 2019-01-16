/* MacroDraw.cc */
/* Created by Enomoto Sanshiro on 19 October 2001. */
/* Last updated by Enomoto Sanshiro on 19 October 2001. */


#include <string>
#include <fstream>
#include "Canvas.hh"
#include "MacroDrawParser.hh"
#include "MacroDraw.hh"

using namespace std;


TMacroDraw::TMacroDraw(void)
{
    _Canvas = new TCanvas();
}

TMacroDraw::~TMacroDraw()
{
    delete _Canvas;
}

void TMacroDraw::Start(int argc, char** argv)
{
    if (argc > 1) {
	try {
	    ExecuteMacro(argv[1]);
	}
	catch (TScriptException &e) {
	    cerr << "ERROR: " << e << endl;
	}
    }
}

void TMacroDraw::ExecuteMacro(const string& FileName) noexcept(false)
{
    ifstream MacroFile(FileName.c_str());
    if (! MacroFile) {
	throw TScriptException(
	    "TMacroDraw::ExecuteMacro()", "unable to open file: " + FileName
	);
    }

    TMacroDrawParser Parser(_Canvas);

    Parser.Parse(MacroFile);
    Parser.Execute("main");
}
