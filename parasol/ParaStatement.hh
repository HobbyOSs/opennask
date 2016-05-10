/* ParaStatement.hh */
/* Created by Enomoto Sanshiro on 27 August 1999. */
/* Last updated by Enomoto Sanshiro on 11 March 2002. */


#ifndef __ParaStatement_hh__
#define __ParaStatement_hh__


#include <string>
#include <vector>
#include <set>
#include <map>
#include "ParaTokenizer.hh"
#include "ParaExpression.hh"
#include "ParaSymbolTable.hh"


class TParaStatement;
class TParaStatementParser;


class TParaStatementTable {
  public:
    TParaStatementTable(void);
    virtual ~TParaStatementTable();
    virtual void Merge(TParaStatementTable* Source);
    virtual void AddStatement(TParaStatement* Statement);
    virtual TParaStatement* CreateStatement(const std::string& FirstToken);
  protected:
    std::map<std::string, TParaStatement*> _StatementTable;
};


class TParaCxxStatementTable: public TParaStatementTable {
  public:    
    TParaCxxStatementTable(void);
    virtual ~TParaCxxStatementTable();
};


class TParaStatementParser {
  public:
    TParaStatementParser(TParaStatementTable* StatementTable, TParaExpressionParser* ExpressionParser);
    virtual ~TParaStatementParser();
    virtual TParaStatement* Parse(TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable) throw(TScriptException);
    TParaExpressionParser* ExpressionParser(void) const;
  protected:
    TParaStatementTable* _StatementTable;
    TParaExpressionParser* _ExpressionParser;
};


class TParaStatement {
  public:
    enum TExecStatus {
	esNormal, esBreak, esContinue, esReturn, esExit, esError
    };
    struct TExecResult {
        TExecResult(void);
        explicit TExecResult(TParaValue& Value);
	TExecStatus ExecStatus;
	TParaValue ReturnValue;
    };
  public:
    TParaStatement(void);
    virtual ~TParaStatement();
    virtual TParaStatement* Clone(void) = 0;
    virtual std::string FirstToken(void) const = 0;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException) = 0;
    virtual TExecResult Execute(TParaSymbolTable* SymbolTable) throw(TScriptException) = 0;
};


class TParaVariableDeclaration {
  public:
    TParaVariableDeclaration(const std::string& TypeName = "");
    virtual ~TParaVariableDeclaration();
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual void Execute(TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual long VariableId(void) const;
 protected:
    long _VariableId;
    std::string _TypeName;
    std::string _VariableName;
    TParaValue* _InitialValue;
    TParaExpression* _ArrayLengthExpression;
    TParaExpression* _InitializeExpression;
    std::vector<TParaExpression*> _ConstructorArgumentList;
    bool _IsArray;
};


class TParaVariableDeclarationStatement: public TParaStatement {
  public:
    TParaVariableDeclarationStatement(void);
    virtual ~TParaVariableDeclarationStatement();
    virtual TParaStatement* Clone(void);
    virtual std::string FirstToken(void) const;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual TExecResult Execute(TParaSymbolTable* SymbolTable) throw(TScriptException);
  protected:
    std::string _TypeName;
    std::string _Position;
    std::vector<TParaVariableDeclaration*> _VariableDeclarationList;
};


class TParaExpressionStatement: public TParaStatement {
  public:
    TParaExpressionStatement(void);
    virtual ~TParaExpressionStatement();
    virtual TParaStatement* Clone(void);
    virtual std::string FirstToken(void) const;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual TExecResult Execute(TParaSymbolTable* SymbolTable) throw(TScriptException);
  protected:
    TParaExpression* _Expression;
};


class TParaComplexStatement: public TParaStatement {
  public:
    TParaComplexStatement(void);
    virtual ~TParaComplexStatement();
    virtual TParaStatement* Clone(void);
    virtual std::string FirstToken(void) const;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual TExecResult Execute(TParaSymbolTable* SymbolTable) throw(TScriptException);
  protected:
    std::vector<TParaStatement*> _StatementList;
};


class TParaEmptyStatement: public TParaStatement {
  public:
    TParaEmptyStatement(void);
    virtual ~TParaEmptyStatement();
    virtual TParaStatement* Clone(void);
    virtual std::string FirstToken(void) const;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual TExecResult Execute(TParaSymbolTable* SymbolTable) throw(TScriptException);
};

class TParaIfStatement: public TParaStatement {
  public:
    TParaIfStatement(void);
    virtual ~TParaIfStatement();
    virtual TParaStatement* Clone(void);
    virtual std::string FirstToken(void) const;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual TExecResult Execute(TParaSymbolTable* SymbolTable) throw(TScriptException);
  protected:
    TParaExpression* _ConditionExpression;
    TParaStatement* _TrueStatement;
    TParaStatement* _FalseStatement;
};


class TParaWhileStatement: public TParaStatement {
  public:
    TParaWhileStatement(void);
    virtual ~TParaWhileStatement();
    virtual TParaStatement* Clone(void);
    virtual std::string FirstToken(void) const;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual TExecResult Execute(TParaSymbolTable* SymbolTable) throw(TScriptException);
  protected:
    TParaExpression* _ConditionExpression;
    TParaStatement* _Statement;
};


class TParaForStatement: public TParaStatement {
  public:
    TParaForStatement(void);
    virtual ~TParaForStatement();
    virtual TParaStatement* Clone(void);
    virtual std::string FirstToken(void) const;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual TExecResult Execute(TParaSymbolTable* SymbolTable) throw(TScriptException);
  protected:
    TParaStatement* _InitializeStatement;
    TParaExpression* _ConditionExpression;
    TParaExpression* _IncrementExpression;
    TParaStatement* _Statement;
};


class TParaForeachStatement: public TParaStatement {
  public:
    TParaForeachStatement(void);
    virtual ~TParaForeachStatement();
    virtual TParaStatement* Clone(void);
    virtual std::string FirstToken(void) const;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual TExecResult Execute(TParaSymbolTable* SymbolTable) throw(TScriptException);
  protected:
    TParaExpression* _VariableExpression;
    TParaExpression* _KeyExpression;
    TParaExpression* _IndexExpression;
    TParaExpression* _ListExpression;
    TParaStatement* _Statement;
};


class TParaBreakStatement: public TParaStatement {
  public:
    TParaBreakStatement(void);
    virtual ~TParaBreakStatement();
    virtual TParaStatement* Clone(void);
    virtual std::string FirstToken(void) const;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual TExecResult Execute(TParaSymbolTable* SymbolTable) throw(TScriptException);
};


class TParaContinueStatement: public TParaStatement {
  public:
    TParaContinueStatement(void);
    virtual ~TParaContinueStatement();
    virtual TParaStatement* Clone(void);
    virtual std::string FirstToken(void) const;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual TExecResult Execute(TParaSymbolTable* SymbolTable) throw(TScriptException);
};


class TParaReturnStatement: public TParaStatement {
  public:
    TParaReturnStatement(void);
    virtual ~TParaReturnStatement();
    virtual TParaStatement* Clone(void);
    virtual std::string FirstToken(void) const;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual TExecResult Execute(TParaSymbolTable* SymbolTable) throw(TScriptException);
  protected:
    TParaExpression* _Expression;
};

class TParaThrowStatement: public TParaStatement {
  public:
    TParaThrowStatement(void);
    virtual ~TParaThrowStatement();
    virtual TParaStatement* Clone(void);
    virtual std::string FirstToken(void) const;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual TExecResult Execute(TParaSymbolTable* SymbolTable) throw(TScriptException);
  protected:
    TParaExpression* _ExceptionExpression;
};

class TParaTryCatchStatement: public TParaStatement {
  public:
    TParaTryCatchStatement(void);
    virtual ~TParaTryCatchStatement();
    virtual TParaStatement* Clone(void);
    virtual std::string FirstToken(void) const;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) throw(TScriptException);
    virtual TExecResult Execute(TParaSymbolTable* SymbolTable) throw(TScriptException);
  protected:
    TParaStatement* _TryStatement;
    TParaVariableDeclaration* _ArgumentDeclaration;
    TParaStatement* _CatchStatement;
};


#endif
