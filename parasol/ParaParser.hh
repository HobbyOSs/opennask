/* ParaParser.hh */
/* Created by Enomoto Sanshiro on 12 April 2001. */
/* Last updated by Enomoto Sanshiro on 12 April 2001. */


#ifndef __ParaParser_hh__
#define __ParaParser_hh__


#include <string>
#include <iostream>
#include "ParaDefs.hh"
#include "ParaTokenizer.hh"
#include "ParaOperator.hh"
#include "ParaExpression.hh"
#include "ParaStatement.hh"
#include "ParaPackage.hh"
#include "ParaStandardLibrary.hh"
#include "ParaContainerLibrary.hh"
#include "ParaMathLibrary.hh"


class TParaParser {
  public:
    TParaParser(void);
    virtual ~TParaParser();
    virtual void Merge(TParaParser* Source);
    virtual void Parse(std::istream& SourceStream) throw(TScriptException);
    virtual TParaValue Execute(const std::string& EntryName = "") throw(TScriptException);
    virtual bool HasEntryOf(const std::string& EntryName) const;
    virtual TParaSymbolTable* GetSymbolTable(void);
    virtual TParaTokenTable* GetTokenTable(void);
    virtual TParaExpressionParser* GetExpressionParser(void);
    virtual TParaStatementParser* GetStatementParser(void);
    virtual TParaPackage* GetPackage(void);
  protected:
    virtual void OnConstruct(void) {}
    virtual TParaObjectPrototypeTable* CreateObjectPrototypeTable(void);
    virtual TParaBuiltinFunctionTable* CreateBuiltinFunctionTable(void);
    virtual TParaTokenTable* CreateTokenTable(void);
    virtual TParaOperatorTable* CreateOperatorTable(void);
    virtual TParaStatementTable* CreateStatementTable(void);
    virtual TParaPackage* CreatePackage(void);
  private:
    virtual void Construct(void);
  protected:
    TParaObjectPrototypeTable* _ObjectPrototypeTable;
    TParaBuiltinFunctionTable* _BuiltinFunctionTable;
    TParaTokenTable* _TokenTable;
    TParaOperatorTable* _OperatorTable;
    TParaStatementTable* _StatementTable;
    TParaSymbolTable* _SymbolTable;
    TParaExpressionParser* _ExpressionParser;
    TParaStatementParser* _StatementParser;
    TParaPackage* _Package;
  private:
    bool _IsConstructed;
};


class TParaStandardParser: public TParaParser {
  public:
    TParaStandardParser(void);
    TParaStandardParser(int argc, char** argv);
    virtual ~TParaStandardParser();
  protected:
    virtual TParaObjectPrototypeTable* CreateObjectPrototypeTable(void);
    virtual TParaBuiltinFunctionTable* CreateBuiltinFunctionTable(void);
    virtual TParaTokenTable* CreateTokenTable(void);
    virtual TParaOperatorTable* CreateOperatorTable(void);
    virtual TParaStatementTable* CreateStatementTable(void);
    virtual TParaPackage* CreatePackage(void);
  protected:
    int _argc;
    char** _argv;
};


#endif

