/* ParaPackage.hh */
/* Created by Enomoto Sanshiro on 1 September 1999. */
/* Last updated by Enomoto Sanshiro on 20 July 2001. */


#ifndef __ParaPackage_hh__
#define __ParaPackage_hh__


#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "ParaTokenizer.hh"
#include "ParaExpression.hh"
#include "ParaSymbolTable.hh"
#include "ParaStatement.hh"
#include "ParaFunction.hh"

class TParaPackageEntry;


class TParaPackage {
  public:
    TParaPackage(void);
    virtual ~TParaPackage();
    virtual void Merge(TParaPackage* Source);
    virtual void AddEntry(TParaPackageEntry* EntryPrototype);
    virtual TParaPackageEntry* CreateEntry(TParaTokenizer* Tokenizer);
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual TParaValue Execute(TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual TParaValue Execute(const std::string& EntryName, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual TParaValue Execute(const std::string& EntryName, const std::vector<TParaValue*>& ArgumentList, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual void ExecuteBareStatements(TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual TParaPackageEntry* GetEntry(const std::string& EntryName);
    virtual const std::vector<TParaPackageEntry*>& EntryList(void) const;
    virtual const std::vector<std::string>& EntryNameList(void) const;
  protected:
    std::vector<TParaPackageEntry*> _EntryPrototypeList;
    std::vector<TParaPackageEntry*> _EntryList;
    std::map<std::string, TParaPackageEntry*> _EntryTable;
    std::vector<std::string> _EntryNameList;
    std::vector<TParaStatement*> _BareStatementList;
  private:
    unsigned _NumberOfProcessedBareStatements;
};


class TParaCxxPackage: public TParaPackage {
  public:
    TParaCxxPackage(void);
    virtual ~TParaCxxPackage();    
};


class TParaPackageEntry {
  public:
    TParaPackageEntry(const std::string& EntryTypeName);
    virtual ~TParaPackageEntry();
    virtual TParaPackageEntry* Clone(void) = 0;
    virtual bool HasEntryWordsOf(TParaTokenizer* Tokenizer) = 0;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException) = 0;
    virtual TParaValue Execute(const std::vector<TParaValue*>& ArgumentList, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual const std::string& EntryTypeName(void) const;
    virtual const std::string& EntryName(void) const;
  protected:
    virtual void SetEntryName(const std::string& EntryName);
  private:
    std::string _EntryTypeName;
    std::string _EntryName;
};


class TParaFunctionEntry: public TParaPackageEntry {
  public:
    TParaFunctionEntry(void);
    virtual ~TParaFunctionEntry();
    virtual TParaPackageEntry* Clone(void);
    virtual bool HasEntryWordsOf(TParaTokenizer* Tokenizer);
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual TParaValue Execute(const std::vector<TParaValue*>& ArgumentList, TParaSymbolTable* SymbolTable) throw(TScriptException);
  protected:
    TParaFunction* _Function;
};


class TParaIncludeEntry: public TParaPackageEntry {
  public:
    TParaIncludeEntry(void);
    virtual ~TParaIncludeEntry();
    virtual TParaPackageEntry* Clone(void);
    virtual bool HasEntryWordsOf(TParaTokenizer* Tokenizer);
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
  protected:
    std::ifstream* _InputFile;
};


#endif
