/* MacroDrawParser.cc */
/* Created by Enomoto Sanshiro on 19 October 2001. */
/* Last updated by Enomoto Sanshiro on 19 October 2001. */


#include "Canvas.hh"
#include "CanvasStatement.hh"
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

TParaTokenTable* TMacroDrawParser::CreateTokenTable(void)
{
    TParaTokenTable* TokenTable;
    TokenTable = TParaStandardParser::CreateTokenTable();

    TokenTable->AddKeyword("macro");

    TokenTable->AddKeyword("line");
    TokenTable->AddKeyword("rect");
    TokenTable->AddKeyword("circle");
    TokenTable->AddKeyword("text");

    return TokenTable;
}

TParaStatementTable* TMacroDrawParser::CreateStatementTable(void)
{
    TParaStatementTable* StatementTable;
    StatementTable = TParaStandardParser::CreateStatementTable();

    StatementTable->AddStatement(new TLineStatement(_Canvas));
    StatementTable->AddStatement(new TRectStatement(_Canvas));
    StatementTable->AddStatement(new TCircleStatement(_Canvas));
    StatementTable->AddStatement(new TTextStatement(_Canvas));

    return StatementTable;
}

TParaPackage* TMacroDrawParser::CreatePackage(void)
{
    TParaPackage* Package = TParaStandardParser::CreatePackage();

    Package->AddEntry(new TMacroEntry());

    return Package;
}



TMacroEntry::TMacroEntry(void)
: TParaPackageEntry("macro")
{
    _Statement = 0;
}

TMacroEntry::~TMacroEntry()
{
    delete _Statement;
}

TParaPackageEntry* TMacroEntry::Clone(void)
{
    return new TMacroEntry();
}

bool TMacroEntry::HasEntryWordsOf(TParaTokenizer* Tokenizer)
{
    return Tokenizer->LookAhead().Is("macro");
}

void TMacroEntry::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false)
{
    Tokenizer->Next().MustBe("macro");
    string MacroName = Tokenizer->Next().RemoveQuotation('"').AsString();

    _Statement = StatementParser->Parse(Tokenizer, SymbolTable);

    SetEntryName(MacroName);
}

TParaValue TMacroEntry::Execute(const vector<TParaValue*>& ArgumentList, TParaSymbolTable* SymbolTable) noexcept(false)
{
    return _Statement->Execute(SymbolTable).ReturnValue;
}
