/* MacroDrawParser.hh */
/* Created by Enomoto Sanshiro on 19 October 2001. */
/* Last updated by Enomoto Sanshiro on 19 October 2001. */


#ifndef __MACRO_DRAW_PARSER_HH
#define __MACRO_DRAW_PARSER_HH


#include "Canvas.hh"
#include "CanvasStatement.hh"
#include "ParaParser.hh"


class TMacroDrawParser: public TParaStandardParser {
  public:
    TMacroDrawParser(TCanvas* Canvas);
    virtual ~TMacroDrawParser();
  protected:
    virtual TParaTokenTable* CreateTokenTable(void);
    virtual TParaStatementTable* CreateStatementTable(void);
    virtual TParaPackage* CreatePackage(void);
  private:
    TCanvas* _Canvas;
};


class TMacroEntry: public TParaPackageEntry {
  public:
    TMacroEntry(void);
    virtual ~TMacroEntry();
    virtual TParaPackageEntry* Clone(void);
    virtual bool HasEntryWordsOf(TParaTokenizer* Tokenizer);
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false);
    virtual TParaValue Execute(const std::vector<TParaValue*>& ArgumentList, TParaSymbolTable* SymbolTable) noexcept(false);
  protected:
    TParaStatement* _Statement;
};


#endif
