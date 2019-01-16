/* solve.cc */
/* Created by Enomoto Sanshiro on 1 September 2000. */
/* Last updated by Enomoto Sanshiro on 27 February 2002. */


#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include "ParaSymbolTable.hh"
#include "ParaExpression.hh"
#include "ParaStandardLibrary.hh"
#include "ParaMathLibrary.hh"

using namespace std;



class TMathFunction {
  public:
    TMathFunction(const string& FunctionText);
    virtual ~TMathFunction();
    virtual void Parse(void) noexcept(false);
    virtual double operator() (double x) const noexcept(false);
  protected:
    string _FunctionText;
    TParaObjectPrototypeTable* _ObjectPrototypeTable;
    TParaBuiltinFunctionTable* _BuiltinFunctionTable;
    TParaSymbolTable* _SymbolTable;
    TParaOperatorTable* _OperatorTable;
    TParaExpression* _Expression;
    TParaValue* _Variable;
};


class TEquationSolver {
  public:
    TEquationSolver(void);
    virtual ~TEquationSolver();
    virtual int Solve(const TMathFunction& Function, double InitialValue, double& Result, double Precision, int MaxTries = 30);
    virtual double Diff(const TMathFunction& Functoin, double x, double eps) noexcept(false);
    string ErrorMessage(void);
  protected:
    string _ErrorMessage;
};



TMathFunction::TMathFunction(const string& FunctionText)
{
    _FunctionText = FunctionText;

    _ObjectPrototypeTable = 0;
    _BuiltinFunctionTable = 0;
    _OperatorTable = 0;
    _SymbolTable = 0;

    _Expression = 0;
    _Variable = 0;
}
 
TMathFunction::~TMathFunction()
{
    delete _Variable;
    delete _Expression;
    delete _OperatorTable;

    delete _SymbolTable;
    delete _BuiltinFunctionTable;
    delete _ObjectPrototypeTable;
}

void TMathFunction::Parse(void) noexcept(false)
{
    _ObjectPrototypeTable = new TParaObjectPrototypeTable();
    _BuiltinFunctionTable = new TParaBuiltinFunctionTable();
    _BuiltinFunctionTable->RegisterAnonymousClass(new TParaMathObject());

    _SymbolTable = new TParaSymbolTable(_ObjectPrototypeTable, _BuiltinFunctionTable);

    _Variable = _SymbolTable->CreateObject("double");
    _SymbolTable->RegisterVariable("x", _Variable);

    _SymbolTable->RegisterVariable("pi", TParaValue(3.141592654));
    _SymbolTable->RegisterVariable("e", TParaValue(2.718281828));

    TParaCxxTokenTable TokenTable;
    _OperatorTable = new TParaCxxOperatorTable();

    istringstream FunctionTextStream(_FunctionText);
    TParaTokenizer Tokenizer(FunctionTextStream, &TokenTable);

    TParaExpressionParser ExpressionParser(_OperatorTable);
    _Expression = ExpressionParser.Parse(&Tokenizer, _SymbolTable);
}

double TMathFunction::operator() (double x) const noexcept(false)
{
    _Variable->Assign(TParaValue(x));
    return _Expression->Evaluate(_SymbolTable).AsDouble();
}



TEquationSolver::TEquationSolver(void)
{
}

TEquationSolver::~TEquationSolver()
{
}

double TEquationSolver::Diff(const TMathFunction& Function, double x, double eps) noexcept(false)
{
    return (Function(x + eps) - Function(x - eps)) / (2 * eps);
}

int TEquationSolver::Solve(const TMathFunction& Function, double InitialValue, double& Result, double Precision, int MaxTries)
{
    double x = InitialValue, x0 = 2 * x + 1;
    int Count = 0;

    try {
	while (fabs(x / x0 - 1.0) > Precision) {
	    x0 = x;
	    x = x0 - Function(x0) / Diff(Function, x0, x0 * Precision);
	    ++Count;
	    
	    if (Count > MaxTries) {
		_ErrorMessage = "not converged";
		return -1;
	    }
	}    
    }
    catch (TScriptException &e) {
	_ErrorMessage = e.Message();
	return -1;
    }

    Result = x;

    return 0;
}

string TEquationSolver::ErrorMessage(void)
{
    return _ErrorMessage;
}



int main(int argc, char** argv)
{
    if (argc < 2) {
	cerr << "usage: " << argv[0];
	cerr << " function [initial_value [precision]]" << endl;
	cerr << "  ex) " << argv[0] << " \"x*x - 2\" 1" << endl;
	return 0;
    }

    string FunctionString = argv[1];
    double InitialValue = 1.0;
    double Precision = 1e-5;

    if (argc > 2) {
	InitialValue = atof(argv[2]);
	if ((InitialValue == 0) && (argv[2][0] != '0')) {
	    cerr << "ERROR: bad initial_value: " << argv[2] << endl;
	    return -1;
	}
    }
    if (argc > 3) {
	Precision = atof(argv[3]);
	if ((Precision == 0) && (argv[3][0] != '0')) {
	    cerr << "ERROR: bad precision: " << argv[3] << endl;
	    return -1;
	}
    }

    TMathFunction Function(FunctionString);
    try {
	Function.Parse();
    }
    catch (TScriptException &e) {
	cerr << "ERROR: " << e << endl;
	return -1;
    }
    
    double Result;
    TEquationSolver Solver;
    if (Solver.Solve(Function, InitialValue, Result, Precision) < 0) {
	cout << Solver.ErrorMessage() << endl;
    }
    else {
	cout << "x = " << Result << endl;
    }

    return 0;
}
