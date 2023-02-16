/* ParaTokenTable.cc */
/* Created by Enomoto Sanshiro on 6 July 1999. */
/* Last updated by Enomoto Sanshiro on 11 March 2002. */


#include <string>
#include <set>
#include <cctype>
#include "ParaTokenTable.hh"

using namespace std;


static const char* KeywordList[] = {
    "int", "long", "float", "double", "string", "list", "pointer", "variant"
};

static const char* SeparatorList[] = {
    ";", ",",
    "(", ")",
    "{", "}",
    "[", "]"
};

static const char* CxxKeywordList[] = {
    "auto", "break", "case", "catch", "char", "class", "const",
    "continue", "default", "delete", "do", "double", "else", "enum",
    "extern", "float", "for", "friend", "goto", "if", "inline",
    "int", "long", "namespace", "new", "operator", "private", "protected",
    "public", "register", "return", "short", "signed", "static",
    "struct", "switch", "templete", "this", "throw", "try", "typedef",
    "typename", "union", "unsigned", "virtual", "void", "volatile", "while"
};

static const char* CxxSeparatorList[] = {
    ";", ",",
    "(", ")",
    "{", "}",
    "[", "]"
};

static const char* CxxOperatorList[] = {
    "+", "-", "*", "/", "%", "=",
    "++", "--",
    "&&", "||", "!",
    ">", "<", ">=", "<=", "==", "!=",
    "+=", "-=", "*=", "/=", "%=",
    ".", "->", "::",
    "&", "|", "&=", "|=",
    "^", "~", "^=", "~=",
    ">>", "<<", ">>=", "<<=",
    "?", ":",
    "new", "delete", "sizeof",
    "typeof", "<+>", "<&>", "<+>=", "=>", "**",
    "#!"
};


TParaTokenTable::TParaTokenTable(void)
{
    int NumberOfKeywords = sizeof(KeywordList) / sizeof(KeywordList[0]);
    for (int i = 0; i < NumberOfKeywords; i++) {
        AddKeyword(KeywordList[i]);
    }

    int NumberOfSeparators = sizeof(SeparatorList) / sizeof(SeparatorList[0]);
    for (int i = 0; i < NumberOfSeparators; i++) {
        AddSeparator(SeparatorList[i]);
    }
}

TParaTokenTable::~TParaTokenTable()
{
    _KeywordSet.clear();
    _OperatorSet.clear();
    _SeparatorSet.clear();
    _AlphabetSet.clear();
    _FollowerAlphabetSet.clear();
    _CommentLimiterTable.clear();
}

void TParaTokenTable::Merge(TParaTokenTable* Source)
{
    _KeywordSet.insert(
	Source->_KeywordSet.begin(), Source->_KeywordSet.end()
    );

    _SeparatorSet.insert(
	Source->_SeparatorSet.begin(), Source->_SeparatorSet.end()
    );

    _OperatorSet.insert(
	Source->_OperatorSet.begin(), Source->_OperatorSet.end()
    );

    _AlphabetSet.insert(
	Source->_AlphabetSet.begin(), Source->_AlphabetSet.end()
    );

    _FollowerAlphabetSet.insert(
	Source->_FollowerAlphabetSet.begin(), Source->_FollowerAlphabetSet.end()
    );

    _CommentLimiterTable.insert(
	Source->_CommentLimiterTable.begin(), Source->_CommentLimiterTable.end()
    );
}

void TParaTokenTable::AddKeyword(const string& Keyword)
{
    _KeywordSet.insert(Keyword);
}

void TParaTokenTable::AddOperator(const string& Operator)
{
    _OperatorSet.insert(Operator);

    // Definition of operators which the tokenizer recognizes is:
    //    Operator:
    //       OperatorCharacter
    //       Operator OperatorCharaceter
    //
    // To let the tokenizer recognize long operators,
    // the following 'sub'-operators have to be registered.

    if (! isalpha(Operator[0]) && Operator[0] != '_') {
	size_t Length = Operator.size();
	if (Length > 1) {
	    string SubOperator = Operator.substr(0, Length - 1);
	    AddOperator(SubOperator);
	}
    }
}

void TParaTokenTable::AddSeparator(const string& Separator)
{
    _SeparatorSet.insert(Separator);
}

void TParaTokenTable::AddAlphabet(char Alphabet)
{
    _AlphabetSet.insert(Alphabet);
}

void TParaTokenTable::AddFollowerAlphabet(char Alphabet)
{
    _FollowerAlphabetSet.insert(Alphabet);
}

void TParaTokenTable::AddCommentLimiter(const string& Limiter, const string& Delimiter)
{
    AddOperator(Limiter);
    _CommentLimiterTable[Limiter] = Delimiter;
}

bool TParaTokenTable::IsKeyword(const string& TokenString) const
{
    return (_KeywordSet.count(TokenString) > 0);
}

bool TParaTokenTable::IsOperator(const string& TokenString) const
{
    return (_OperatorSet.count(TokenString) > 0);
}

bool TParaTokenTable::IsSeparator(const string& TokenString) const
{
    return (_SeparatorSet.count(TokenString) > 0);
}

bool TParaTokenTable::IsAlphabet(const char& Character) const
{
    return isalpha(Character) || (_AlphabetSet.count(Character) > 0);
}

bool TParaTokenTable::IsFollowerAlphabet(const char& Character) const
{
    return isalnum(Character) || (_AlphabetSet.count(Character) > 0) || (_FollowerAlphabetSet.count(Character) > 0);
}

bool TParaTokenTable::IsCommentLimiter(const string& TokenString) const
{
    return (_CommentLimiterTable.count(TokenString) > 0);
}

string TParaTokenTable::CommentDelimiterFor(const string& Limiter) const
{
    map<string, string>::const_iterator Iterator;
    Iterator = _CommentLimiterTable.find(Limiter);

    if (Iterator != _CommentLimiterTable.end()) {
	return (*Iterator).second;
    }
    else {
	return string("");
    }
}



TParaCxxTokenTable::TParaCxxTokenTable()
{
    int i;
    int NumberOfKeywords = sizeof(CxxKeywordList) / sizeof(CxxKeywordList[0]);
    for (i = 0; i < NumberOfKeywords; i++) {
        AddKeyword(CxxKeywordList[i]);
    }

    int NumberOfSeparators = sizeof(CxxSeparatorList) / sizeof(CxxSeparatorList[0]);
    for (i = 0; i < NumberOfSeparators; i++) {
        AddSeparator(CxxSeparatorList[i]);
    }

    int NumberOfOperators = sizeof(CxxOperatorList) / sizeof(CxxOperatorList[0]);
    for (i = 0; i < NumberOfOperators; i++) {
        AddOperator(CxxOperatorList[i]);
    }

    AddAlphabet('_');

    AddCommentLimiter("/*", "*/");
    AddCommentLimiter("//", "\n");
    AddCommentLimiter("#!", "\n");
}


TParaCxxTokenTable::~TParaCxxTokenTable()
{
    _KeywordSet.clear();
    _OperatorSet.clear();
    _SeparatorSet.clear();
    _AlphabetSet.clear();
    _FollowerAlphabetSet.clear();
    _CommentLimiterTable.clear();
}
