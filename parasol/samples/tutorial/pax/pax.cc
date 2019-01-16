/* pax.cc */
/* Created by Enomoto Sanshiro on 1 March 2002. */
/* Last updated by Enomoto Sanshiro on 1 March 2002. */


#include <iostream>
#include <string>
#include <map>
#include "ParaTokenizer.hh"

using namespace std;


class TPaxHandler {
  public:
    typedef map<string, string> TAttributeList;
  public:
    TPaxHandler(void);
    virtual ~TPaxHandler();
    virtual void StartElement(const string& Name, const TAttributeList& AttributeList);
    virtual void EndElement(const string& Name);
};


class TPaxParser {
  public:
    TPaxParser(TPaxHandler* Handler);
    virtual ~TPaxParser();
    virtual void Parse(istream& InputStream) noexcept(false);
  protected:
    TPaxHandler* _Handler;
};




TPaxHandler::TPaxHandler(void)
{
}

TPaxHandler::~TPaxHandler()
{
}

void TPaxHandler::StartElement(const string& Name, const TPaxHandler::TAttributeList& AttributeList)
{
    cout << "start [" << Name;

    TAttributeList::const_iterator Attribute;
    for (Attribute = AttributeList.begin(); Attribute != AttributeList.end(); Attribute++) {
	cout << ", " << Attribute->first << "=\'" << Attribute->second << "\'";
    }
	
    cout << "]" << endl;
}

void TPaxHandler::EndElement(const string& Name)
{
    cout << "end [" << Name << "]" << endl;
}



TPaxParser::TPaxParser(TPaxHandler* Handler)
{
    _Handler = Handler;
}

TPaxParser::~TPaxParser()
{
}

void TPaxParser::Parse(istream& InputStream) noexcept(false)
{
    TParaTokenTable TokenTable;
    TokenTable.AddOperator("<");
    TokenTable.AddOperator(">");
    TokenTable.AddOperator("</");
    TokenTable.AddOperator("/>");
    TokenTable.AddOperator("=");
    TokenTable.AddAlphabet('_');
    TokenTable.AddFollowerAlphabet('-');
    TokenTable.AddFollowerAlphabet('.');
    TokenTable.AddCommentLimiter("<!--", "-->");

    TParaTokenizer Tokenizer(InputStream, &TokenTable);
    
    TParaToken Token;
    while (! (Token = Tokenizer.Next()).IsEmpty()) {
	if (Token.Is("<")) {
	    string Name = Tokenizer.Next().AsString();
	    
	    TPaxHandler::TAttributeList AttributeList;
	    while (Tokenizer.LookAhead().IsIdentifier()) {
		string AttributeName = Tokenizer.Next().AsString();
		Tokenizer.Next().MustBe("=");
		string Value = Tokenizer.Next().RemoveQuotation('\"').AsString();
		AttributeList[AttributeName] = Value;
	    }
	    
	    _Handler->StartElement(Name, AttributeList);
	    
	    if (Tokenizer.LookAhead().Is("/>")) {
		_Handler->EndElement(Name);
	    }
	    else {
		Tokenizer.Next().MustBe(">");
	    }
	}
	else if (Token.Is("</")) {
	    string Name = Tokenizer.Next().AsString();
	    Tokenizer.Next().MustBe(">");
	    
	    _Handler->EndElement(Name);
	}
	else {
	    ;
	}
    }
}


int main(int argc, char** argv)
{
    TPaxHandler Handler;
    TPaxParser Parser(&Handler);

    try {
	Parser.Parse(cin);
    }
    catch (TScriptException &e) {
        cerr << "ERROR: " << e << endl;
    }
    
    return 0;
}
