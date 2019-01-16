/* ParaExpression.hh */
/* Created by Enomoto Sanshiro on 22 August 1999. */
/* Last updated by Enomoto Sanshiro on 3 July 2002. */


#ifndef __ParaExpression_hh__
#define __ParaExpression_hh__


#include <iostream>
#include <string>
#include <vector>
#include "ParaTokenizer.hh"
#include "ParaObject.hh"
#include "ParaValue.hh"
#include "ParaOperator.hh"
#include "ParaBuiltinFunction.hh"
#include "ParaSymbolTable.hh"


class TParaExpression;
class TParaFunctionCallNode;


class TParaExpressionParser {
  public:
    TParaExpressionParser(TParaOperatorTable* OperatorTable);
    virtual ~TParaExpressionParser();
    virtual TParaExpression* Parse(TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable, int PriorityRank = -1) noexcept(false);
  public:
    virtual std::vector<TParaExpression*> ParseExpressionList(TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable, const std::string& StartToken, const std::string& StopToken, const std::string& SeparatorToken) noexcept(false);
  public:
    virtual TParaExpression* ParsePrepositional(TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable) noexcept(false);
    virtual TParaExpression* ParseElement(TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable) noexcept(false);
    virtual TParaExpression* ParsePostpositional(TParaExpression* Expression, TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable) noexcept(false);
    virtual TParaExpression* ParseTypeCast(TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable) noexcept(false);
    virtual TParaExpression* ParseTemporaryObjectCreation(TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable) noexcept(false);
    virtual TParaFunctionCallNode* ParseFunctionCall(TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable) noexcept(false);
    virtual TParaExpression* ParseListExpression(TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable) noexcept(false);
    virtual TParaExpression* ParseMethodInvocation(TParaExpression* ObjectNode, TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable) noexcept(false);
    virtual TParaExpression* ParseArrayReference(TParaExpression* ObjectNode, TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable) noexcept(false);
    virtual TParaExpression* ParseTableReference(TParaExpression* ObjectNode, TParaTokenizer* Tokenizer, TParaSymbolTable* SymbolTable) noexcept(false);
  protected:  
    TParaOperatorTable* _OperatorTable;
};


class TParaExpression {
  public:
    TParaExpression();
    virtual ~TParaExpression();
    virtual TParaValue& Evaluate(TParaSymbolTable* SymbolTable) noexcept(false) = 0;
    virtual void Dump(std::ostream &os, int IndentLevel = 0) const;
    virtual void SetLineNumber(long LineNumber);
    virtual std::string Position(void) const;
  protected:
    virtual void DumpThis(std::ostream &os) const = 0;
  protected:
    TParaExpression* _LeftNode;
    TParaExpression* _RightNode;
    long _LineNumber;
};


class TParaOperatorNode: public TParaExpression {
  public:
    TParaOperatorNode(TParaOperator* Operator, TParaExpression* LeftNode, TParaExpression* RightNode);
    virtual ~TParaOperatorNode();
    virtual TParaValue& Evaluate(TParaSymbolTable* SymbolTable) noexcept(false);
  protected:
    virtual void DumpThis(std::ostream &os) const;
  protected:
    TParaOperator* _Operator;
    TParaValue _Value;
};


class TParaTypeCastNode: public TParaExpression {
  public:
    TParaTypeCastNode(const std::string& TypeName, TParaExpression* RightNode);
    virtual ~TParaTypeCastNode();
    virtual TParaValue& Evaluate(TParaSymbolTable* SymbolTable) noexcept(false);
  protected:
    virtual void DumpThis(std::ostream &os) const;
  protected:
    std::string _TypeName;
    TParaValue _Value;
};


class TParaLiteralNode: public TParaExpression {
  public:
    TParaLiteralNode(const TParaValue& Value);
    virtual ~TParaLiteralNode();
    virtual TParaValue& Evaluate(TParaSymbolTable* SymbolTable) noexcept(false);
  protected:
    virtual void DumpThis(std::ostream &os) const;
  protected:
    TParaValue _Value;
};


class TParaVariableNode: public TParaExpression {
  public:
    TParaVariableNode(long VariableId);
    virtual ~TParaVariableNode();
    virtual TParaValue& Evaluate(TParaSymbolTable* SymbolTable) noexcept(false);
  protected:
    virtual void DumpThis(std::ostream &os) const;
  protected:
    long _VariableId;
};


class TParaListNode: public TParaExpression {
  public:
    TParaListNode(const std::vector<TParaExpression*>& KeyExpressionList, const std::vector<TParaExpression*>& ValueExpressionList);
    virtual ~TParaListNode();
    virtual TParaValue& Evaluate(TParaSymbolTable* SymbolTable) noexcept(false);
  protected:
    virtual void DumpThis(std::ostream &os) const;
  protected:
    std::vector<TParaExpression*> _KeyExpressionList;
    std::vector<TParaExpression*> _ValueExpressionList;
    TParaValue _Value;
};


class TParaFunctionCallNode: public TParaExpression {
  public:
    TParaFunctionCallNode(long FunctionId, std::vector<TParaExpression*>& ArgumentExpressionList);
    virtual ~TParaFunctionCallNode();
    virtual TParaValue& Evaluate(TParaSymbolTable* SymbolTable) noexcept(false);
  public:
    virtual void EvaluateArguments(TParaSymbolTable* SymbolTable) noexcept(false);
    virtual TParaValue& ExecuteFunction(TParaSymbolTable* SymbolTable) noexcept(false);
  protected:
    virtual void DumpThis(std::ostream &os) const;
  protected:
    TParaValue _Value;
    long _FunctionId;
    TParaBuiltinFunctionTable* _BuiltinFunctionTable;
    std::vector<TParaExpression*> _ArgumentExpressionList;
    std::vector<TParaValue*> _ArgumentList;
};


class TParaMethodInvocationNode: public TParaFunctionCallNode {
  public:
    TParaMethodInvocationNode(TParaExpression* ObjectExpression, long FunctionId, std::vector<TParaExpression*>& ArgumentExpressionList);
    virtual ~TParaMethodInvocationNode();
    virtual TParaValue& Evaluate(TParaSymbolTable* SymbolTable) noexcept(false);
  protected:
    int _MethodId;
    std::string _MethodName;
    TParaExpression* _ObjectExpression;
    std::vector<TParaExpression*> _ArgumentExpressionList;
};


class TParaPropertyAccessNode: public TParaExpression {
  public:
    TParaPropertyAccessNode(TParaExpression* ObjectExpression, const std::string& PropertyName);
    virtual ~TParaPropertyAccessNode();
    virtual TParaValue& Evaluate(TParaSymbolTable* SymbolTable) noexcept(false);
  protected:
    virtual void DumpThis(std::ostream &os) const;
  protected:
    std::string _PropertyName;
    TParaExpression* _ObjectExpression;
    TParaValue _Value;
};


class TParaTemporaryObjectCreationNode: public TParaFunctionCallNode {
  public:
    TParaTemporaryObjectCreationNode(const std::string& TypeName, std::vector<TParaExpression*>& ArgumentExpressionList);
    virtual ~TParaTemporaryObjectCreationNode();
    virtual TParaValue& Evaluate(TParaSymbolTable* SymbolTable) noexcept(false);
  protected:
    std::string _TypeName;
    std::vector<TParaExpression*> _ArgumentExpressionList;
};


class TParaArrayReferenceNode: public TParaExpression {
  public:
    TParaArrayReferenceNode(TParaExpression* ObjectExpression, TParaExpression* IndexExpression);
    virtual ~TParaArrayReferenceNode();
    virtual TParaValue& Evaluate(TParaSymbolTable* SymbolTable) noexcept(false);
  protected:
    virtual void DumpThis(std::ostream &os) const;
  protected:
    virtual TParaValue& ListItemOf(TParaValue& ListValue, int Index) noexcept(false);
    virtual TParaValue& PartialListOf(TParaValue& ListValue, TParaValue& IndexListValue) noexcept(false);
    virtual TParaValue& StringItemOf(TParaValue& StringValue, int Index) noexcept(false);
  protected:
    TParaExpression* _ObjectExpression;
    TParaExpression* _IndexExpression;
    TParaValue _Value;
};


class TParaTableReferenceNode: public TParaExpression {
  public:
    TParaTableReferenceNode(TParaExpression* ObjectExpression, TParaExpression* IndexExpression);
    virtual ~TParaTableReferenceNode();
    virtual TParaValue& Evaluate(TParaSymbolTable* SymbolTable) noexcept(false);
  protected:
    virtual TParaValue& PartialListOf(TParaValue& ListValue, TParaValue& IndexListValue) noexcept(false);
    virtual void DumpThis(std::ostream &os) const;
  protected:
    TParaExpression* _ObjectExpression;
    TParaExpression* _IndexExpression;
    TParaValue _Value;
};


#endif
