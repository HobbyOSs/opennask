/* CanvasStatement.cc */
/* Created by Enomoto Sanshiro on 19 October 2001. */
/* Last updated by Enomoto Sanshiro on 19 October 2001. */


#include "Canvas.hh"
#include "ParaObject.hh"
#include "CanvasStatement.hh"

using namespace std;



TLineStatement::TLineStatement(TCanvas* Canvas)
{
    _Canvas = Canvas;

    _X0 = 0;
    _Y0 = 0;
    _X1 = 0;
    _Y1 = 0;
}

TLineStatement::~TLineStatement()
{
    delete _X0;
    delete _Y0;
    delete _X1;
    delete _Y1;
}

TParaStatement* TLineStatement::Clone(void)
{
    return new TLineStatement(_Canvas);
}

string TLineStatement::FirstToken(void) const
{
    return string("line");
}

void TLineStatement::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false)
{
    TParaExpressionParser* ExpressionParser;
    ExpressionParser = StatementParser->ExpressionParser();

    Tokenizer->Next().MustBe("line");
    Tokenizer->Next().MustBe("(");

    _X0 = ExpressionParser->Parse(Tokenizer, SymbolTable);
    Tokenizer->Next().MustBe(",");
    _Y0 = ExpressionParser->Parse(Tokenizer, SymbolTable);

    Tokenizer->Next().MustBe(")");
    Tokenizer->Next().MustBe("-");
    Tokenizer->Next().MustBe("(");

    _X1 = ExpressionParser->Parse(Tokenizer, SymbolTable);
    Tokenizer->Next().MustBe(",");
    _Y1 = ExpressionParser->Parse(Tokenizer, SymbolTable);

    Tokenizer->Next().MustBe(")");
    Tokenizer->Next().MustBe(";");
}

TParaStatement::TExecResult TLineStatement::Execute(TParaSymbolTable* SymbolTable) noexcept(false)
{
    float X0 = _X0->Evaluate(SymbolTable).AsDouble();
    float Y0 = _Y0->Evaluate(SymbolTable).AsDouble();
    float X1 = _X1->Evaluate(SymbolTable).AsDouble();
    float Y1 = _Y1->Evaluate(SymbolTable).AsDouble();

    _Canvas->DrawLine(X0, Y0, X1, Y1);

    return TExecResult();
}



TRectStatement::TRectStatement(TCanvas* Canvas)
{
    _Canvas = Canvas;

    _X0 = 0;
    _Y0 = 0;
    _X1 = 0;
    _Y1 = 0;
}

TRectStatement::~TRectStatement()
{
    delete _X0;
    delete _Y0;
    delete _X1;
    delete _Y1;
}

TParaStatement* TRectStatement::Clone(void)
{
    return new TRectStatement(_Canvas);
}

string TRectStatement::FirstToken(void) const
{
    return string("rect");
}

void TRectStatement::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false)
{
    TParaExpressionParser* ExpressionParser;
    ExpressionParser = StatementParser->ExpressionParser();

    Tokenizer->Next().MustBe("rect");
    Tokenizer->Next().MustBe("(");

    _X0 = ExpressionParser->Parse(Tokenizer, SymbolTable);
    Tokenizer->Next().MustBe(",");
    _Y0 = ExpressionParser->Parse(Tokenizer, SymbolTable);

    Tokenizer->Next().MustBe(")");
    Tokenizer->Next().MustBe("-");
    Tokenizer->Next().MustBe("(");

    _X1 = ExpressionParser->Parse(Tokenizer, SymbolTable);
    Tokenizer->Next().MustBe(",");
    _Y1 = ExpressionParser->Parse(Tokenizer, SymbolTable);

    Tokenizer->Next().MustBe(")");
    Tokenizer->Next().MustBe(";");
}

TParaStatement::TExecResult TRectStatement::Execute(TParaSymbolTable* SymbolTable) noexcept(false)
{
    float X0 = _X0->Evaluate(SymbolTable).AsDouble();
    float Y0 = _Y0->Evaluate(SymbolTable).AsDouble();
    float X1 = _X1->Evaluate(SymbolTable).AsDouble();
    float Y1 = _Y1->Evaluate(SymbolTable).AsDouble();

    _Canvas->DrawRect(X0, Y0, X1, Y1);

    return TExecResult();
}



TCircleStatement::TCircleStatement(TCanvas* Canvas)
{
    _Canvas = Canvas;

    _X = 0;
    _Y = 0;
    _Radius = 0;
}

TCircleStatement::~TCircleStatement()
{
    delete _X;
    delete _Y;
    delete _Radius;
}

TParaStatement* TCircleStatement::Clone(void)
{
    return new TCircleStatement(_Canvas);
}

string TCircleStatement::FirstToken(void) const
{
    return string("circle");
}

void TCircleStatement::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false)
{
    TParaExpressionParser* ExpressionParser;
    ExpressionParser = StatementParser->ExpressionParser();

    Tokenizer->Next().MustBe("circle");
    Tokenizer->Next().MustBe("(");

    _X = ExpressionParser->Parse(Tokenizer, SymbolTable);
    Tokenizer->Next().MustBe(",");
    _Y = ExpressionParser->Parse(Tokenizer, SymbolTable);

    Tokenizer->Next().MustBe(")");
    Tokenizer->Next().MustBe(",");

    _Radius = ExpressionParser->Parse(Tokenizer, SymbolTable);

    Tokenizer->Next().MustBe(";");
}

TParaStatement::TExecResult TCircleStatement::Execute(TParaSymbolTable* SymbolTable) noexcept(false)
{
    float X = _X->Evaluate(SymbolTable).AsDouble();
    float Y = _Y->Evaluate(SymbolTable).AsDouble();
    float Radius = _Radius->Evaluate(SymbolTable).AsDouble();

    _Canvas->DrawCircle(X, Y, Radius);

    return TExecResult();
}



TTextStatement::TTextStatement(TCanvas* Canvas)
{
    _Canvas = Canvas;

    _X = 0;
    _Y = 0;
    _Text = 0;
}

TTextStatement::~TTextStatement()
{
    delete _X;
    delete _Y;
    delete _Text;
}

TParaStatement* TTextStatement::Clone(void)
{
    return new TTextStatement(_Canvas);
}

string TTextStatement::FirstToken(void) const
{
    return string("text");
}

void TTextStatement::Parse(TParaTokenizer* Tokenizer, TParaStatementParser* StatementParser, TParaSymbolTable* SymbolTable) noexcept(false)
{
    TParaExpressionParser* ExpressionParser;
    ExpressionParser = StatementParser->ExpressionParser();

    Tokenizer->Next().MustBe("text");
    Tokenizer->Next().MustBe("(");

    _X = ExpressionParser->Parse(Tokenizer, SymbolTable);
    Tokenizer->Next().MustBe(",");
    _Y = ExpressionParser->Parse(Tokenizer, SymbolTable);

    Tokenizer->Next().MustBe(")");
    Tokenizer->Next().MustBe(",");

    _Text = ExpressionParser->Parse(Tokenizer, SymbolTable);

    Tokenizer->Next().MustBe(";");
}

TParaStatement::TExecResult TTextStatement::Execute(TParaSymbolTable* SymbolTable) noexcept(false)
{
    float X = _X->Evaluate(SymbolTable).AsDouble();
    float Y = _Y->Evaluate(SymbolTable).AsDouble();
    string Text = _Text->Evaluate(SymbolTable).AsString();

    _Canvas->DrawText(X, Y, Text);

    return TExecResult();
}
