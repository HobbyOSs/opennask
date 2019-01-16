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
	string FileName = argv[1];
	string MacroName;
	if (argc > 2) {
	    MacroName = argv[2];
	}
	
	try {
	    ExecuteMacro(FileName, MacroName);
	}
	catch (TScriptException &e) {
	    cerr << "ERROR: " << e << endl;
	}
    }
}

void TMacroDraw::ExecuteMacro(const string& FileName, const string& MacroName) noexcept(false)
{
    ifstream MacroFile(FileName.c_str());
    if (! MacroFile) {
	throw TScriptException(
	    "TMacroDraw::ExecuteMacro()", "unable to open file: " + FileName
	    );
    }

    TMacroDrawParser Parser(_Canvas);
    Parser.Parse(MacroFile);
    
    TParaPackage* Package = Parser.GetPackage();
    TParaPackageEntry* Entry = Package->GetEntry(MacroName);

    if ((Entry != 0) && (Entry->EntryTypeName() == "macro")) {
	Parser.Execute(MacroName);
    }
    else {
	cout << "macro list: " << endl;

	const vector<TParaPackageEntry*>& EntryList = Package->EntryList();
	for (unsigned i = 0; i < EntryList.size(); i++) {
	    if (EntryList[i]->EntryTypeName() == "macro") {
		cout << "  " << EntryList[i]->EntryName() << endl;
	    }
	}
    }
}
