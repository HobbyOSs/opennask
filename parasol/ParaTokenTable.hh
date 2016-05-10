/* ParaTokenTable.hh */
/* Created by Enomoto Sanshiro on 6 July 1999. */
/* Last updated by Enomoto Sanshiro on 11 March 2002. */


#ifndef __ParaTokenTable_hh__
#define __ParaTokenTable_hh__


#include <string>
#include <set>
#include <map>


class TParaTokenTable {
  public:
    TParaTokenTable(void);
    virtual ~TParaTokenTable();
    virtual void Merge(TParaTokenTable* Source);
    virtual void AddKeyword(const std::string& Keyword);
    virtual void AddOperator(const std::string& Operator);    
    virtual void AddSeparator(const std::string& Separator);
    virtual void AddAlphabet(char Alphabet);
    virtual void AddFollowerAlphabet(char Alphabet);
    virtual void AddCommentLimiter(const std::string& Limiter, const std::string& Delimiter);
    virtual bool IsKeyword(const std::string& TokenString) const;
    virtual bool IsOperator(const std::string& TokenString) const;
    virtual bool IsSeparator(const std::string& TokenString) const;
    virtual bool IsAlphabet(const char& Character) const;
    virtual bool IsFollowerAlphabet(const char& Character) const;
    virtual bool IsCommentLimiter(const std::string& TokenString) const;
    virtual std::string CommentDelimiterFor(const std::string& CommentLimiter) const;
  protected:
    std::set<std::string> _KeywordSet;
    std::set<std::string> _OperatorSet;
    std::set<std::string> _SeparatorSet;
    std::set<char> _AlphabetSet;
    std::set<char> _FollowerAlphabetSet;
    std::map<std::string, std::string> _CommentLimiterTable;
};


class TParaCxxTokenTable: public TParaTokenTable {
  public:
    TParaCxxTokenTable(void);
    virtual ~TParaCxxTokenTable();
};


#endif
