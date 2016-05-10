/* ParaFunction.hh */
/* Created by Enomoto Sanshiro on 1 September 1999. */
/* Last updated by Enomoto Sanshiro on 12 April 2000. */


#ifndef __ParaFunction_hh__
#define __ParaFunction_hh__

#include <string>
#include <vector>
#include "ParaTokenizer.hh"
#include "ParaExpression.hh"
#include "ParaSymbolTable.hh"
#include "ParaStatement.hh"



class TParaFunction {
  public:
    TParaFunction(void);
    virtual ~TParaFunction();
    virtual std::string Name(void);
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException) = 0;
    virtual TParaValue Execute(const std::vector<TParaValue*>& ArgumentList, TParaSymbolTable* SymbolTable) throw(TScriptException);
  protected:
    virtual void ProcessArguments(const std::vector<TParaValue*>& ArgumentList, TParaSymbolTable* SymbolTable) throw(TScriptException);
  protected:
    virtual void SetName(const std::string& Name);
    virtual void SetReturnValue(TParaValue* ReturnValue);
    virtual void AddArgumentDeclaration(TParaVariableDeclaration* ArgumentDeclaration);
    virtual void SetStatement(TParaStatement* Statement);
  private:
    std::string _Name;
    std::vector<TParaVariableDeclaration*> _ArgumentDeclarationList;
    TParaStatement* _Statement;
    TParaValue* _ReturnValue;
};



class TParaCxxFunction: public TParaFunction {
  public:
    TParaCxxFunction(void);
    virtual ~TParaCxxFunction();
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
  protected:
    virtual void ParseArgumentDeclaration(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
};


#endif
