/* ParaOperator.hh */
/* Created by Enomoto Sanshiro on 8 July 1999. */
/* Last updated by Enomoto Sanshiro on 3 May 2002. */


#ifndef __ParaOperator_hh__
#define __ParaOperator_hh__


#include <string>
#include <vector>
#include <map>
#include "ParaObject.hh"
#include "ParaValue.hh"
#include "ParaTokenizer.hh"
#include "ParaSymbolTable.hh"


class TParaExpressionParser;
class TParaExpression;
class TParaOperator;


class TParaOperatorPriority {
  public:
    explicit TParaOperatorPriority(int PriorityRank = -1);
    TParaOperatorPriority(const std::string& BaseOperatorSymbol, int PriorityOffset, int TemporaryPriorityRank = -1);
    TParaOperatorPriority(const TParaOperatorPriority& Priority);
    virtual ~TParaOperatorPriority();
    virtual TParaOperatorPriority& operator= (const TParaOperatorPriority& Priority);
    virtual void SetPriorityRank(int PriorityRank);
    virtual int PriorityRank(void) const;
    virtual const std::string& BaseOperatorSymbol(void) const;
    virtual int PriorityOffset(void) const;
  protected:
    int _PriorityRank;
    std::string _BaseOperatorSymbol;
    int _PriorityOffset;
};


class TParaOperatorTable {
  public:
    TParaOperatorTable(void);
    virtual ~TParaOperatorTable();
    virtual void Merge(TParaOperatorTable* Source);
    virtual void AddOperator(TParaOperator* Operator, int PriorityRank);
    virtual void AddOperator(TParaOperator* Operator, const TParaOperatorPriority& Priority);
    virtual void AddPrepositionalOperator(TParaOperator* Operator);
    virtual void AddPostpositionalOperator(TParaOperator* Operator);
    virtual void AddElementaryOperator(TParaOperator* Operator);
    virtual TParaOperator* CreateOperator(const std::string& Symbol);
    virtual TParaOperator* CreatePrepositionalOperator(const std::string& Symbol);
    virtual TParaOperator* CreatePostpositionalOperator(const std::string& Symbol);
    virtual TParaOperator* CreateElementaryOperator(const std::string& Symbol);
    virtual const TParaOperatorPriority& PriorityOf(const std::string& Symbol);
    virtual int PriorityRankOf(const std::string& Symbol);
    virtual int HighestPriorityRank(void) const;
    virtual int LowestPriorityRank(void) const;
  protected:
    std::map<std::string, TParaOperator*> _OperatorTable;
    std::map<std::string, TParaOperator*> _PrepositionalOperatorTable;
    std::map<std::string, TParaOperator*> _PostpositionalOperatorTable;
    std::map<std::string, TParaOperator*> _ElementaryOperatorTable;
    std::map<std::string, TParaOperatorPriority> _PriorityTable;
    std::map<std::string, int> _PriorityRankTable;
    int _HighestPriorityRank;
    int _LowestPriorityRank;
};


class TParaCxxOperatorTable: public TParaOperatorTable {
  public:
    TParaCxxOperatorTable(void);
    virtual ~TParaCxxOperatorTable();
};


class TParaOperator {
  public:
    TParaOperator(void);
    virtual ~TParaOperator();
    virtual TParaOperator* Clone(void) const = 0;
    virtual std::string Symbol(void) const = 0;
    virtual std::string Name(void) const = 0;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaExpressionParser* ExpressionParser, TParaSymbolTable* SymbolTable) noexcept(false);
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false) = 0;
    virtual bool IsLeftAssociative(void) const;
  protected:
    virtual TParaValue& EvaluateList(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorNew: public TParaOperator {
  public:
    TParaOperatorNew(void);
    virtual ~TParaOperatorNew();
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaExpressionParser* ExpressionParser, TParaSymbolTable* SymbolTable) noexcept(false);
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
  protected:
    std::string _TypeName;
    TParaExpression* _LengthExpression;
    std::vector<TParaExpression*> _ArgumentList;
};


class TParaOperatorDelete: public TParaOperator {
  public:
    TParaOperatorDelete(void);
    virtual ~TParaOperatorDelete();
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaExpressionParser* ExpressionParser, TParaSymbolTable* SymbolTable) noexcept(false);
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
  protected:
    bool _IsForArray;
};


class TParaOperatorVariableAccess: public TParaOperator {
  public:
    TParaOperatorVariableAccess(void);
    virtual ~TParaOperatorVariableAccess();
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaExpressionParser* ExpressionParser, TParaSymbolTable* SymbolTable) noexcept(false);
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
  protected:
    TParaExpression* _VariableNameExpression;
    std::string _VariableName;
    long _VariableId;
    static TParaSymbolTable* _MySymbolTable;
};


class TParaOperatorListGenerate: public TParaOperator {
  public:
    TParaOperatorListGenerate(void);
    virtual ~TParaOperatorListGenerate();
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaExpressionParser* ExpressionParser, TParaSymbolTable* SymbolTable) noexcept(false);
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
  protected:
    TParaExpression* _StartValueExpression;
    TParaExpression* _EndValueExpression;
    TParaExpression* _StepValueExpression;
    std::string _Separator;
};


class TParaOperatorSizeOf: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorTypeOf: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorKeys: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorPointerReference: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorAddress: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorIncrement: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorDecrement: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorPostpositionalIncrement: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorPostpositionalDecrement: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorSignPlus: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorSignMinus: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorNot: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorBitReverse: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorMultiple: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorDivide: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorModulo: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorAdd: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorSubtract: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorConcatenate: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorLeftShift: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorRightShift: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorGreaterThan: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorLessThan: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorGreaterEqual: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorLessEqual: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorEqual: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorNotEqual: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorAnd: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorOr: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorBitAnd: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorBitXor: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorBitOr: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorListAnd: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorAssign: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
    virtual bool IsLeftAssociative(void) const;
};


class TParaOperatorAssignSum: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
    virtual bool IsLeftAssociative(void) const;
};


class TParaOperatorAssignDifference: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
    virtual bool IsLeftAssociative(void) const;
};


class TParaOperatorAssignProduct: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
    virtual bool IsLeftAssociative(void) const;
};


class TParaOperatorAssignQuotient: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
    virtual bool IsLeftAssociative(void) const;
};


class TParaOperatorAssignRemainder: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
    virtual bool IsLeftAssociative(void) const;
};


class TParaOperatorAssignConcatenation: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
    virtual bool IsLeftAssociative(void) const;
};


class TParaOperatorFactorial: public TParaOperator {
  public:
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
};


class TParaOperatorPower: public TParaOperator {
  public:
    TParaOperatorPower(void);
    virtual ~TParaOperatorPower();
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual void Parse(TParaTokenizer* Tokenizer, TParaExpressionParser* ExpressionParser, TParaSymbolTable* SymbolTable) noexcept(false);
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& Result) noexcept(false);
  protected:
    TParaExpression* _PowerExpression;
};


#endif
