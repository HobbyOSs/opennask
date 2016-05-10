/* yaci.cc */
/* Created by Enomoto Sanshiro on 30 August 1999. */
/* Last updated by Enomoto Sanshiro on 24 August 2000. */


#include <iostream>
#include <fstream>
#include <cstdlib>
#include "ParaParser.hh"

using namespace std;


int main(int argc, char** argv)
{
    if (argc < 2) {
	cerr << "Usage: " << argv[0] << " SourceFileName" << endl;
	return 0;
    }
    ifstream SourceFile(argv[1]);
    if (! SourceFile) {
	cerr << "ERROR: unable to open " << argv[1] << endl;
	return 0;
    }

    TParaStandardParser Parser(argc - 1, argv + 1);
    try {
        Parser.Parse(SourceFile);
    }
    catch (TScriptException &e) {
	cerr << "ERROR: " << e << endl;
	return EXIT_FAILURE;
    }

    TParaValue Result;
    try {
	if (Parser.HasEntryOf("main")) {
	    Result = Parser.Execute("main");
	}
	else {
	    Result = Parser.Execute();
	}
    }
    catch (TScriptException &e) {
        cerr << "ERROR: " << e << endl;
    }
    
    return Result.AsLong();
}
