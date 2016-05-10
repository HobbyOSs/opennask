/* BitOperators.hh */
/* Created by Enomoto Sanshiro on 24 August 2000. */
/* Last updated by Enomoto Sanshiro on 24 August 2000. */


#ifndef __BitOperators_hh__
#define __BitOperators_hh__


#include "ParaOperator.hh"
#include "ParaSymbolTable.hh"



class TOperatorBitGenerate: public TParaOperator {
  public:
    TOperatorBitGenerate(void);
    virtual ~TOperatorBitGenerate();
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& ReturnValue) throw(TScriptException); 
};


class TOperatorBitFill: public TParaOperator {
  public:
    TOperatorBitFill(void);
    virtual ~TOperatorBitFill();
    virtual TParaOperator* Clone(void) const;
    virtual std::string Symbol(void) const;
    virtual std::string Name(void) const;
    virtual TParaValue& Evaluate(TParaValue& Left, TParaValue& Right, TParaSymbolTable* SymbolTable, TParaValue& ReturnValue) throw(TScriptException); 
};


#endif

