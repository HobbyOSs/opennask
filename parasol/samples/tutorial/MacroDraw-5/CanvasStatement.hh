/* CanvasStatement.hh */
/* Created by Enomoto Sanshiro on 19 October 2001. */
/* Last updated by Enomoto Sanshiro on 19 October 2001. */


#ifndef __CANVAS_STATEMENT_HH
#define __CANVAS_STATEMENT_HH


#include "Canvas.hh"
#include "ParaStatement.hh"


class TLineStatement: public TParaStatement {
  public:
    TLineStatement(TCanvas* Canvas);
    virtual ~TLineStatement();
    virtual TParaStatement* Clone(void);
    virtual std::string FirstToken(void) const;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual TExecResult Execute(TParaSymbolTable* SymbolTable) throw(TScriptException);
  private:
    TCanvas* _Canvas;
    TParaExpression* _X0;
    TParaExpression* _Y0;
    TParaExpression* _X1;
    TParaExpression* _Y1;
};


class TRectStatement: public TParaStatement {
  public:
    TRectStatement(TCanvas* Canvas);
    virtual ~TRectStatement();
    virtual TParaStatement* Clone(void);
    virtual std::string FirstToken(void) const;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual TExecResult Execute(TParaSymbolTable* SymbolTable) throw(TScriptException);
  private:
    TCanvas* _Canvas;
    TParaExpression* _X0;
    TParaExpression* _Y0;
    TParaExpression* _X1;
    TParaExpression* _Y1;
};


class TCircleStatement: public TParaStatement {
  public:
    TCircleStatement(TCanvas* Canvas);
    virtual ~TCircleStatement();
    virtual TParaStatement* Clone(void);
    virtual std::string FirstToken(void) const;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual TExecResult Execute(TParaSymbolTable* SymbolTable) throw(TScriptException);
  private:
    TCanvas* _Canvas;
    TParaExpression* _X;
    TParaExpression* _Y;
    TParaExpression* _Radius;
};


class TTextStatement: public TParaStatement {
  public:
    TTextStatement(TCanvas* Canvas);
    virtual ~TTextStatement();
    virtual TParaStatement* Clone(void);
    virtual std::string FirstToken(void) const;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual TExecResult Execute(TParaSymbolTable* SymbolTable) throw(TScriptException);
  private:
    TCanvas* _Canvas;
    TParaExpression* _X;
    TParaExpression* _Y;
    TParaExpression* _Text;
};


#endif
