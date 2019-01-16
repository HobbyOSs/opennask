/* RepeatUntilStatement.hh */
/* Created by Enomoto Sanshiro on 24 August 2000. */
/* Last updated by Enomoto Sanshiro on 24 August 2000. */


#ifndef __RepeatUntilStatement_hh__
#define __RepeatUntilStatement_hh__


#include <string>
#include "ParaStatement.hh"
#include "ParaExpression.hh"
#include "ParaOperator.hh"


class TRepeatUntilStatement: public TParaStatement {
  public:
    TRepeatUntilStatement(void);
    virtual ~TRepeatUntilStatement();
    virtual TParaStatement* Clone(void);
    virtual std::string FirstToken(void) const;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false);
    virtual TExecResult Execute(TParaSymbolTable* SymbolTable) noexcept(false);
  protected:
    TParaExpression* _ConditionExpression;
    TParaStatement* _Statement;
};



#endif
