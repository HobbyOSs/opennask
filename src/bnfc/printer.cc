/*** BNFC-Generated Pretty Printer and Abstract Syntax Viewer ***/

#include <string>
#include "printer.hh"
#define INDENT_WIDTH 2


//You may wish to change render
void PrintAbsyn::render(Char c)
{
  if (c == '{')
  {
     onEmptyLine();
     bufAppend(c);
     _n_ = _n_ + INDENT_WIDTH;
     bufAppend('\n');
     indent();
  }
  else if (c == '(' || c == '[')
     bufAppend(c);
  else if (c == ')' || c == ']')
  {
     removeTrailingWhitespace();
     bufAppend(c);
     bufAppend(' ');
  }
  else if (c == '}')
  {
     _n_ = _n_ - INDENT_WIDTH;
     onEmptyLine();
     bufAppend(c);
     bufAppend('\n');
     indent();
  }
  else if (c == ',')
  {
     removeTrailingWhitespace();
     bufAppend(c);
     bufAppend(' ');
  }
  else if (c == ';')
  {
     removeTrailingWhitespace();
     bufAppend(c);
     bufAppend('\n');
     indent();
  }
  else if (c == ' ') bufAppend(c);
  else if (c == 0) return;
  else
  {
     bufAppend(c);
     bufAppend(' ');
  }
}

void PrintAbsyn::render(String s)
{
  render(s.c_str());
}

bool allIsSpace(const char *s)
{
  char c;
  while ((c = *s++))
    if (! isspace(c)) return false;
  return true;
}

void PrintAbsyn::render(const char *s)
{
  if (*s) /* C string not empty */
  {
    if (allIsSpace(s)) {
      backup();
      bufAppend(s);
    } else {
      bufAppend(s);
      bufAppend(' ');
    }
  }
}

void PrintAbsyn::indent()
{
  int n = _n_;
  while (--n >= 0)
    bufAppend(' ');
}

void PrintAbsyn::backup()
{
  if (cur_ && buf_[cur_ - 1] == ' ')
    buf_[--cur_] = 0;
}

void PrintAbsyn::removeTrailingSpaces()
{
  while (cur_ && buf_[cur_ - 1] == ' ') --cur_;
  buf_[cur_] = 0;
}

void PrintAbsyn::removeTrailingWhitespace()
{
  while (cur_ && (buf_[cur_ - 1] == ' ' || buf_[cur_ - 1] == '\n')) --cur_;
  buf_[cur_] = 0;
}

void PrintAbsyn::onEmptyLine()
{
  removeTrailingSpaces();
  if (cur_ && buf_[cur_ - 1 ] != '\n') bufAppend('\n');
  indent();
}

PrintAbsyn::PrintAbsyn(void)
{
  _i_ = 0; _n_ = 0;
  buf_ = 0;
  bufReset();
}

PrintAbsyn::~PrintAbsyn(void)
{
}

char *PrintAbsyn::print(Visitable *v)
{
  _i_ = 0; _n_ = 0;
  bufReset();
  v->accept(this);
  return buf_;
}

void PrintAbsyn::visitProgram(Program *p) {} //abstract class

void PrintAbsyn::visitProg(Prog *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  _i_ = 0; visitListStatement(p->liststatement_);

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitListStatement(ListStatement *liststatement)
{
  iterListStatement(liststatement->begin(), liststatement->end());
}

void PrintAbsyn::iterListStatement(ListStatement::const_iterator i, ListStatement::const_iterator j)
{
  if (i == j) return;
  if (i == j-1)
  { /* last */
    (*i)->accept(this);
  }
  else
  { /* cons */
    (*i)->accept(this); iterListStatement(i+1, j);
  }
}

void PrintAbsyn::visitStatement(Statement *p) {} //abstract class

void PrintAbsyn::visitLabelStmt(LabelStmt *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  visitIdent(p->ident_);
  render(':');

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitDeclareStmt(DeclareStmt *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  visitIdent(p->ident_);
  render("EQU");
  _i_ = 0; p->exp_->accept(this);

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitConfigStmt(ConfigStmt *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render('[');
  _i_ = 0; p->configtype_->accept(this);
  visitString(p->string_);
  render(']');

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitMnemonicStmt(MnemonicStmt *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  _i_ = 0; p->opcode_->accept(this);
  _i_ = 0; visitListMnemonicArgs(p->listmnemonicargs_);

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitListMnemonicArgs(ListMnemonicArgs *listmnemonicargs)
{
  iterListMnemonicArgs(listmnemonicargs->begin(), listmnemonicargs->end());
}

void PrintAbsyn::iterListMnemonicArgs(ListMnemonicArgs::const_iterator i, ListMnemonicArgs::const_iterator j)
{
  if (i == j) return;
  if (i == j-1)
  { /* last */
    (*i)->accept(this);
  }
  else
  { /* cons */
    (*i)->accept(this); render(','); iterListMnemonicArgs(i+1, j);
  }
}

void PrintAbsyn::visitMnemonicArgs(MnemonicArgs *p) {} //abstract class

void PrintAbsyn::visitMnemoArgs(MnemoArgs *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  _i_ = 0; p->exp_->accept(this);

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitExp(Exp *p) {} //abstract class

void PrintAbsyn::visitEqExp(EqExp *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  _i_ = 0; p->factor_1->accept(this);
  render("==");
  _i_ = 0; p->factor_2->accept(this);

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitNeqExp(NeqExp *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  _i_ = 0; p->factor_1->accept(this);
  render("!=");
  _i_ = 0; p->factor_2->accept(this);

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitLtExp(LtExp *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  _i_ = 0; p->factor_1->accept(this);
  render('<');
  _i_ = 0; p->factor_2->accept(this);

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitGtExp(GtExp *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  _i_ = 0; p->factor_1->accept(this);
  render('>');
  _i_ = 0; p->factor_2->accept(this);

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitLteExp(LteExp *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  _i_ = 0; p->factor_1->accept(this);
  render("<=");
  _i_ = 0; p->factor_2->accept(this);

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitGteExp(GteExp *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  _i_ = 0; p->factor_1->accept(this);
  render(">=");
  _i_ = 0; p->factor_2->accept(this);

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitPlusExp(PlusExp *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  _i_ = 0; p->factor_1->accept(this);
  render('+');
  _i_ = 0; p->factor_2->accept(this);

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitMinusExp(MinusExp *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  _i_ = 0; p->factor_1->accept(this);
  render('-');
  _i_ = 0; p->factor_2->accept(this);

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitMulExp(MulExp *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  _i_ = 0; p->factor_1->accept(this);
  render('*');
  _i_ = 0; p->factor_2->accept(this);

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitDivExp(DivExp *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  _i_ = 0; p->factor_1->accept(this);
  render('/');
  _i_ = 0; p->factor_2->accept(this);

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitModExp(ModExp *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  _i_ = 0; p->factor_1->accept(this);
  render('%');
  _i_ = 0; p->factor_2->accept(this);

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitImmExp(ImmExp *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  _i_ = 0; p->factor_->accept(this);

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitFactor(Factor *p) {} //abstract class

void PrintAbsyn::visitNumberFactor(NumberFactor *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  visitInteger(p->integer_);

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitHexFactor(HexFactor *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  visitHex(p->hex_);

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitIdentFactor(IdentFactor *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  visitIdent(p->ident_);

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitStringFactor(StringFactor *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  visitString(p->string_);

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitConfigType(ConfigType *p) {} //abstract class

void PrintAbsyn::visitBitsConfig(BitsConfig *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("BITS");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitInstConfig(InstConfig *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("INSTRSET");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOptiConfig(OptiConfig *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("OPTIMIZE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitFormConfig(FormConfig *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FORMAT");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitPaddConfig(PaddConfig *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("PADDING");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitPadsConfig(PadsConfig *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("PADSET");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitSectConfig(SectConfig *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SECTION");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitAbsoConfig(AbsoConfig *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("ABSOLUTE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitFileConfig(FileConfig *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FILE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcode(Opcode *p) {} //abstract class

void PrintAbsyn::visitOpcodesAAA(OpcodesAAA *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("AAA");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesAAD(OpcodesAAD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("AAD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesAAS(OpcodesAAS *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("AAS");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesAAM(OpcodesAAM *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("AAM");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesADC(OpcodesADC *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("ADC");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesADD(OpcodesADD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("ADD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesAND(OpcodesAND *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("AND");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesALIGN(OpcodesALIGN *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("ALIGN");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesALIGNB(OpcodesALIGNB *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("ALIGNB");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesARPL(OpcodesARPL *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("ARPL");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesBOUND(OpcodesBOUND *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("BOUND");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesBSF(OpcodesBSF *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("BSF");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesBSR(OpcodesBSR *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("BSR");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesBSWAP(OpcodesBSWAP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("BSWAP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesBT(OpcodesBT *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("BT");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesBTC(OpcodesBTC *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("BTC");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesBTR(OpcodesBTR *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("BTR");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesBTS(OpcodesBTS *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("BTS");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesCALL(OpcodesCALL *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("CALL");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesCBW(OpcodesCBW *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("CBW");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesCDQ(OpcodesCDQ *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("CDQ");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesCLC(OpcodesCLC *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("CLC");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesCLD(OpcodesCLD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("CLD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesCLI(OpcodesCLI *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("CLI");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesCLTS(OpcodesCLTS *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("CLTS");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesCMC(OpcodesCMC *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("CMC");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesCMP(OpcodesCMP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("CMP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesCMPSB(OpcodesCMPSB *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("CMPSB");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesCMPSD(OpcodesCMPSD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("CMPSD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesCMPSW(OpcodesCMPSW *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("CMPSW");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesCMPXCHG(OpcodesCMPXCHG *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("CMPXCHG");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesCPUID(OpcodesCPUID *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("CPUID");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesCWD(OpcodesCWD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("CWD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesCWDE(OpcodesCWDE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("CWDE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesDAA(OpcodesDAA *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("DAA");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesDAS(OpcodesDAS *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("DAS");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesDB(OpcodesDB *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("DB");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesDD(OpcodesDD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("DD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesDEC(OpcodesDEC *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("DEC");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesDIV(OpcodesDIV *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("DIV");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesDQ(OpcodesDQ *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("DQ");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesDT(OpcodesDT *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("DT");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesDW(OpcodesDW *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("DW");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesEND(OpcodesEND *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("END");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesENTER(OpcodesENTER *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("ENTER");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesEXTERN(OpcodesEXTERN *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("EXTERN");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesF2XM1(OpcodesF2XM1 *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("F2XM1");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFABS(OpcodesFABS *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FABS");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFADD(OpcodesFADD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FADD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFADDP(OpcodesFADDP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FADDP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFBLD(OpcodesFBLD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FBLD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFBSTP(OpcodesFBSTP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FBSTP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFCHS(OpcodesFCHS *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FCHS");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFCLEX(OpcodesFCLEX *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FCLEX");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFCOM(OpcodesFCOM *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FCOM");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFCOMP(OpcodesFCOMP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FCOMP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFCOMPP(OpcodesFCOMPP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FCOMPP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFCOS(OpcodesFCOS *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FCOS");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFDECSTP(OpcodesFDECSTP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FDECSTP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFDISI(OpcodesFDISI *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FDISI");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFDIV(OpcodesFDIV *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FDIV");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFDIVP(OpcodesFDIVP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FDIVP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFDIVR(OpcodesFDIVR *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FDIVR");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFDIVRP(OpcodesFDIVRP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FDIVRP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFENI(OpcodesFENI *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FENI");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFFREE(OpcodesFFREE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FFREE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFIADD(OpcodesFIADD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FIADD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFICOM(OpcodesFICOM *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FICOM");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFICOMP(OpcodesFICOMP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FICOMP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFIDIV(OpcodesFIDIV *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FIDIV");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFIDIVR(OpcodesFIDIVR *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FIDIVR");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFILD(OpcodesFILD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FILD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFIMUL(OpcodesFIMUL *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FIMUL");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFINCSTP(OpcodesFINCSTP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FINCSTP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFINIT(OpcodesFINIT *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FINIT");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFIST(OpcodesFIST *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FIST");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFISTP(OpcodesFISTP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FISTP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFISUB(OpcodesFISUB *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FISUB");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFISUBR(OpcodesFISUBR *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FISUBR");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFLD(OpcodesFLD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FLD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFLD1(OpcodesFLD1 *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FLD1");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFLDCW(OpcodesFLDCW *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FLDCW");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFLDENV(OpcodesFLDENV *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FLDENV");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFLDL2E(OpcodesFLDL2E *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FLDL2E");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFLDL2T(OpcodesFLDL2T *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FLDL2T");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFLDLG2(OpcodesFLDLG2 *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FLDLG2");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFLDLN2(OpcodesFLDLN2 *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FLDLN2");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFLDPI(OpcodesFLDPI *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FLDPI");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFLDZ(OpcodesFLDZ *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FLDZ");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFMUL(OpcodesFMUL *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FMUL");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFMULP(OpcodesFMULP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FMULP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFNCLEX(OpcodesFNCLEX *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FNCLEX");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFNDISI(OpcodesFNDISI *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FNDISI");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFNENI(OpcodesFNENI *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FNENI");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFNINIT(OpcodesFNINIT *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FNINIT");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFNOP(OpcodesFNOP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FNOP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFNSAVE(OpcodesFNSAVE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FNSAVE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFNSTCW(OpcodesFNSTCW *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FNSTCW");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFNSTENV(OpcodesFNSTENV *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FNSTENV");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFNSTSW(OpcodesFNSTSW *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FNSTSW");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFPATAN(OpcodesFPATAN *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FPATAN");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFPTAN(OpcodesFPTAN *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FPTAN");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFPREM(OpcodesFPREM *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FPREM");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFPREM1(OpcodesFPREM1 *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FPREM1");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFRNDINT(OpcodesFRNDINT *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FRNDINT");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFRSTOR(OpcodesFRSTOR *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FRSTOR");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFSAVE(OpcodesFSAVE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FSAVE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFSCALE(OpcodesFSCALE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FSCALE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFSETPM(OpcodesFSETPM *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FSETPM");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFSIN(OpcodesFSIN *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FSIN");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFSINCOS(OpcodesFSINCOS *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FSINCOS");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFSQRT(OpcodesFSQRT *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FSQRT");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFST(OpcodesFST *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FST");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFSTCW(OpcodesFSTCW *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FSTCW");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFSTENV(OpcodesFSTENV *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FSTENV");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFSTP(OpcodesFSTP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FSTP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFSTSW(OpcodesFSTSW *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FSTSW");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFSUB(OpcodesFSUB *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FSUB");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFSUBP(OpcodesFSUBP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FSUBP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFSUBR(OpcodesFSUBR *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FSUBR");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFSUBRP(OpcodesFSUBRP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FSUBRP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFTST(OpcodesFTST *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FTST");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFUCOM(OpcodesFUCOM *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FUCOM");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFUCOMP(OpcodesFUCOMP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FUCOMP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFUCOMPP(OpcodesFUCOMPP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FUCOMPP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFXAM(OpcodesFXAM *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FXAM");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFXCH(OpcodesFXCH *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FXCH");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFXTRACT(OpcodesFXTRACT *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FXTRACT");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFYL2X(OpcodesFYL2X *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FYL2X");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesFYL2XP1(OpcodesFYL2XP1 *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("FYL2XP1");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesHLT(OpcodesHLT *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("HLT");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesIDIV(OpcodesIDIV *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("IDIV");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesIMUL(OpcodesIMUL *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("IMUL");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesIN(OpcodesIN *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("IN");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesINC(OpcodesINC *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("INC");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesINCO(OpcodesINCO *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("INCO");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesINSB(OpcodesINSB *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("INSB");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesINSD(OpcodesINSD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("INSD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesINSW(OpcodesINSW *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("INSW");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesINT(OpcodesINT *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("INT");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesINT3(OpcodesINT3 *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("INT3");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesINTO(OpcodesINTO *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("INTO");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesINVD(OpcodesINVD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("INVD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesINVLPG(OpcodesINVLPG *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("INVLPG");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesIRET(OpcodesIRET *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("IRET");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesIRETD(OpcodesIRETD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("IRETD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesIRETW(OpcodesIRETW *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("IRETW");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJA(OpcodesJA *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JA");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJAE(OpcodesJAE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JAE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJB(OpcodesJB *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JB");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJBE(OpcodesJBE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JBE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJC(OpcodesJC *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JC");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJCXZ(OpcodesJCXZ *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JCXZ");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJE(OpcodesJE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJECXZ(OpcodesJECXZ *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JECXZ");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJG(OpcodesJG *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JG");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJGE(OpcodesJGE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JGE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJL(OpcodesJL *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JL");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJLE(OpcodesJLE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JLE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJMP(OpcodesJMP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JMP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJNA(OpcodesJNA *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JNA");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJNAE(OpcodesJNAE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JNAE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJNB(OpcodesJNB *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JNB");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJNBE(OpcodesJNBE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JNBE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJNC(OpcodesJNC *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JNC");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJNE(OpcodesJNE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JNE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJNG(OpcodesJNG *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JNG");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJNGE(OpcodesJNGE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JNGE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJNL(OpcodesJNL *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JNL");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJNLE(OpcodesJNLE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JNLE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJNO(OpcodesJNO *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JNO");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJNP(OpcodesJNP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JNP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJNS(OpcodesJNS *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JNS");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJNZ(OpcodesJNZ *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JNZ");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJO(OpcodesJO *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JO");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJP(OpcodesJP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJPE(OpcodesJPE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JPE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJPO(OpcodesJPO *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JPO");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJS(OpcodesJS *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JS");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesJZ(OpcodesJZ *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("JZ");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLAHF(OpcodesLAHF *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LAHF");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLAR(OpcodesLAR *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LAR");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLDS(OpcodesLDS *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LDS");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLEA(OpcodesLEA *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LEA");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLEAVE(OpcodesLEAVE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LEAVE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLES(OpcodesLES *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LES");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLFS(OpcodesLFS *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LFS");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLGDT(OpcodesLGDT *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LGDT");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLGS(OpcodesLGS *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LGS");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLIDT(OpcodesLIDT *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LIDT");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLLDT(OpcodesLLDT *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LLDT");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLMSW(OpcodesLMSW *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LMSW");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLOCK(OpcodesLOCK *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LOCK");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLODSB(OpcodesLODSB *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LODSB");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLODSD(OpcodesLODSD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LODSD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLODSW(OpcodesLODSW *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LODSW");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLOOP(OpcodesLOOP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LOOP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLOOPE(OpcodesLOOPE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LOOPE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLOOPNE(OpcodesLOOPNE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LOOPNE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLOOPNZ(OpcodesLOOPNZ *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LOOPNZ");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLOOPZ(OpcodesLOOPZ *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LOOPZ");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLSL(OpcodesLSL *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LSL");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLSS(OpcodesLSS *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LSS");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesLTR(OpcodesLTR *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("LTR");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesMOV(OpcodesMOV *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("MOV");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesMOVSB(OpcodesMOVSB *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("MOVSB");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesMOVSD(OpcodesMOVSD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("MOVSD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesMOVSW(OpcodesMOVSW *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("MOVSW");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesMOVSX(OpcodesMOVSX *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("MOVSX");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesMOVZX(OpcodesMOVZX *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("MOVZX");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesMUL(OpcodesMUL *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("MUL");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesNEG(OpcodesNEG *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("NEG");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesNOP(OpcodesNOP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("NOP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesNOT(OpcodesNOT *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("NOT");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesOR(OpcodesOR *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("OR");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesORG(OpcodesORG *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("ORG");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesOUT(OpcodesOUT *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("OUT");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesOUTSB(OpcodesOUTSB *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("OUTSB");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesOUTSD(OpcodesOUTSD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("OUTSD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesOUTSW(OpcodesOUTSW *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("OUTSW");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesPOP(OpcodesPOP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("POP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesPOPA(OpcodesPOPA *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("POPA");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesPOPAD(OpcodesPOPAD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("POPAD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesPOPAW(OpcodesPOPAW *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("POPAW");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesPOPF(OpcodesPOPF *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("POPF");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesPOPFD(OpcodesPOPFD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("POPFD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesPOPFW(OpcodesPOPFW *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("POPFW");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesPUSH(OpcodesPUSH *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("PUSH");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesPUSHA(OpcodesPUSHA *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("PUSHA");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesPUSHD(OpcodesPUSHD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("PUSHD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesPUSHAD(OpcodesPUSHAD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("PUSHAD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesPUSHAW(OpcodesPUSHAW *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("PUSHAW");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesPUSHF(OpcodesPUSHF *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("PUSHF");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesPUSHFD(OpcodesPUSHFD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("PUSHFD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesPUSHFW(OpcodesPUSHFW *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("PUSHFW");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesRCL(OpcodesRCL *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("RCL");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesRCR(OpcodesRCR *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("RCR");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesRDMSR(OpcodesRDMSR *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("RDMSR");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesRDPMC(OpcodesRDPMC *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("RDPMC");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesREP(OpcodesREP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("REP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesREPE(OpcodesREPE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("REPE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesREPNE(OpcodesREPNE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("REPNE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesREPNZ(OpcodesREPNZ *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("REPNZ");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesREPZ(OpcodesREPZ *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("REPZ");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesRESB(OpcodesRESB *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("RESB");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesRESD(OpcodesRESD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("RESD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesRESQ(OpcodesRESQ *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("RESQ");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesREST(OpcodesREST *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("REST");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesRESW(OpcodesRESW *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("RESW");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesRET(OpcodesRET *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("RET");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesRETF(OpcodesRETF *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("RETF");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesRETN(OpcodesRETN *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("RETN");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesROL(OpcodesROL *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("ROL");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesROR(OpcodesROR *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("ROR");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesRSM(OpcodesRSM *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("RSM");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSAHF(OpcodesSAHF *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SAHF");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSAL(OpcodesSAL *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SAL");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSAR(OpcodesSAR *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SAR");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSBB(OpcodesSBB *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SBB");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSCASB(OpcodesSCASB *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SCASB");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSCASD(OpcodesSCASD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SCASD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSCASW(OpcodesSCASW *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SCASW");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETA(OpcodesSETA *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETA");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETAE(OpcodesSETAE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETAE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETB(OpcodesSETB *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETB");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETBE(OpcodesSETBE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETBE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETC(OpcodesSETC *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETC");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETE(OpcodesSETE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETG(OpcodesSETG *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETG");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETGE(OpcodesSETGE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETGE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETL(OpcodesSETL *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETL");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETLE(OpcodesSETLE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETLE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETNA(OpcodesSETNA *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETNA");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETNAE(OpcodesSETNAE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETNAE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETNB(OpcodesSETNB *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETNB");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETNBE(OpcodesSETNBE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETNBE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETNC(OpcodesSETNC *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETNC");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETNE(OpcodesSETNE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETNE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETNG(OpcodesSETNG *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETNG");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETNGE(OpcodesSETNGE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETNGE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETNL(OpcodesSETNL *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETNL");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETNLE(OpcodesSETNLE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETNLE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETNO(OpcodesSETNO *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETNO");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETNP(OpcodesSETNP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETNP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETNS(OpcodesSETNS *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETNS");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETNZ(OpcodesSETNZ *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETNZ");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETO(OpcodesSETO *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETO");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETP(OpcodesSETP *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETP");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETPE(OpcodesSETPE *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETPE");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETPO(OpcodesSETPO *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETPO");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETS(OpcodesSETS *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETS");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSETZ(OpcodesSETZ *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SETZ");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSGDT(OpcodesSGDT *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SGDT");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSHL(OpcodesSHL *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SHL");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSHLD(OpcodesSHLD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SHLD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSHR(OpcodesSHR *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SHR");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSHRD(OpcodesSHRD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SHRD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSIDT(OpcodesSIDT *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SIDT");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSLDT(OpcodesSLDT *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SLDT");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSMSW(OpcodesSMSW *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SMSW");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSTC(OpcodesSTC *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("STC");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSTD(OpcodesSTD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("STD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSTI(OpcodesSTI *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("STI");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSTOSB(OpcodesSTOSB *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("STOSB");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSTOSD(OpcodesSTOSD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("STOSD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSTOSW(OpcodesSTOSW *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("STOSW");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSTR(OpcodesSTR *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("STR");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesSUB(OpcodesSUB *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("SUB");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesTEST(OpcodesTEST *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("TEST");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesTIMES(OpcodesTIMES *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("TIMES");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesUD2(OpcodesUD2 *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("UD2");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesVERR(OpcodesVERR *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("VERR");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesVERW(OpcodesVERW *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("VERW");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesWAIT(OpcodesWAIT *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("WAIT");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesWBINVD(OpcodesWBINVD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("WBINVD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesWRMSR(OpcodesWRMSR *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("WRMSR");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesXADD(OpcodesXADD *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("XADD");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesXCHG(OpcodesXCHG *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("XCHG");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesXLATB(OpcodesXLATB *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("XLATB");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitOpcodesXOR(OpcodesXOR *p)
{
  int oldi = _i_;
  if (oldi > 0) render(_L_PAREN);

  render("XOR");

  if (oldi > 0) render(_R_PAREN);
  _i_ = oldi;
}

void PrintAbsyn::visitInteger(Integer i)
{
  char tmp[20];
  sprintf(tmp, "%d", i);
  render(tmp);
}

void PrintAbsyn::visitDouble(Double d)
{
  char tmp[24];
  sprintf(tmp, "%.15g", d);
  render(tmp);
}

void PrintAbsyn::visitChar(Char c)
{
  char tmp[4];
  sprintf(tmp, "'%c'", c);
  render(tmp);
}

void PrintAbsyn::visitString(String s)
{
  bufAppend('\"');
  bufAppend(s);
  bufAppend('\"');
  bufAppend(' ');
}

void PrintAbsyn::visitIdent(String s)
{
  render(s);
}

void PrintAbsyn::visitHex(String s)
{
  render(s);
}


ShowAbsyn::ShowAbsyn(void)
{
  buf_ = 0;
  bufReset();
}

ShowAbsyn::~ShowAbsyn(void)
{
}

char *ShowAbsyn::show(Visitable *v)
{
  bufReset();
  v->accept(this);
  return buf_;
}

void ShowAbsyn::visitProgram(Program *p) {} //abstract class

void ShowAbsyn::visitProg(Prog *p)
{
  bufAppend('(');
  bufAppend("Prog");
  bufAppend(' ');
  bufAppend('[');
  if (p->liststatement_)  p->liststatement_->accept(this);
  bufAppend(']');
  bufAppend(')');
}
void ShowAbsyn::visitListStatement(ListStatement *liststatement)
{
  for (ListStatement::const_iterator i = liststatement->begin() ; i != liststatement->end() ; ++i)
  {
    (*i)->accept(this);
    if (i != liststatement->end() - 1) bufAppend(", ");
  }
}

void ShowAbsyn::visitStatement(Statement *p) {} //abstract class

void ShowAbsyn::visitLabelStmt(LabelStmt *p)
{
  bufAppend('(');
  bufAppend("LabelStmt");
  bufAppend(' ');
  visitIdent(p->ident_);
  bufAppend(' ');
  bufAppend(')');
}
void ShowAbsyn::visitDeclareStmt(DeclareStmt *p)
{
  bufAppend('(');
  bufAppend("DeclareStmt");
  bufAppend(' ');
  visitIdent(p->ident_);
  bufAppend(' ');
  bufAppend('[');
  if (p->exp_)  p->exp_->accept(this);
  bufAppend(']');
  bufAppend(')');
}
void ShowAbsyn::visitConfigStmt(ConfigStmt *p)
{
  bufAppend('(');
  bufAppend("ConfigStmt");
  bufAppend(' ');
  bufAppend('[');
  if (p->configtype_)  p->configtype_->accept(this);
  bufAppend(']');
  bufAppend(' ');
  visitString(p->string_);
  bufAppend(' ');
  bufAppend(')');
}
void ShowAbsyn::visitMnemonicStmt(MnemonicStmt *p)
{
  bufAppend('(');
  bufAppend("MnemonicStmt");
  bufAppend(' ');
  bufAppend('[');
  if (p->opcode_)  p->opcode_->accept(this);
  bufAppend(']');
  bufAppend(' ');
  bufAppend('[');
  if (p->listmnemonicargs_)  p->listmnemonicargs_->accept(this);
  bufAppend(']');
  bufAppend(')');
}
void ShowAbsyn::visitListMnemonicArgs(ListMnemonicArgs *listmnemonicargs)
{
  for (ListMnemonicArgs::const_iterator i = listmnemonicargs->begin() ; i != listmnemonicargs->end() ; ++i)
  {
    (*i)->accept(this);
    if (i != listmnemonicargs->end() - 1) bufAppend(", ");
  }
}

void ShowAbsyn::visitMnemonicArgs(MnemonicArgs *p) {} //abstract class

void ShowAbsyn::visitMnemoArgs(MnemoArgs *p)
{
  bufAppend('(');
  bufAppend("MnemoArgs");
  bufAppend(' ');
  bufAppend('[');
  if (p->exp_)  p->exp_->accept(this);
  bufAppend(']');
  bufAppend(')');
}
void ShowAbsyn::visitExp(Exp *p) {} //abstract class

void ShowAbsyn::visitEqExp(EqExp *p)
{
  bufAppend('(');
  bufAppend("EqExp");
  bufAppend(' ');
  p->factor_1->accept(this);
  bufAppend(' ');
  p->factor_2->accept(this);
  bufAppend(')');
}
void ShowAbsyn::visitNeqExp(NeqExp *p)
{
  bufAppend('(');
  bufAppend("NeqExp");
  bufAppend(' ');
  p->factor_1->accept(this);
  bufAppend(' ');
  p->factor_2->accept(this);
  bufAppend(')');
}
void ShowAbsyn::visitLtExp(LtExp *p)
{
  bufAppend('(');
  bufAppend("LtExp");
  bufAppend(' ');
  p->factor_1->accept(this);
  bufAppend(' ');
  p->factor_2->accept(this);
  bufAppend(')');
}
void ShowAbsyn::visitGtExp(GtExp *p)
{
  bufAppend('(');
  bufAppend("GtExp");
  bufAppend(' ');
  p->factor_1->accept(this);
  bufAppend(' ');
  p->factor_2->accept(this);
  bufAppend(')');
}
void ShowAbsyn::visitLteExp(LteExp *p)
{
  bufAppend('(');
  bufAppend("LteExp");
  bufAppend(' ');
  p->factor_1->accept(this);
  bufAppend(' ');
  p->factor_2->accept(this);
  bufAppend(')');
}
void ShowAbsyn::visitGteExp(GteExp *p)
{
  bufAppend('(');
  bufAppend("GteExp");
  bufAppend(' ');
  p->factor_1->accept(this);
  bufAppend(' ');
  p->factor_2->accept(this);
  bufAppend(')');
}
void ShowAbsyn::visitPlusExp(PlusExp *p)
{
  bufAppend('(');
  bufAppend("PlusExp");
  bufAppend(' ');
  p->factor_1->accept(this);
  bufAppend(' ');
  p->factor_2->accept(this);
  bufAppend(')');
}
void ShowAbsyn::visitMinusExp(MinusExp *p)
{
  bufAppend('(');
  bufAppend("MinusExp");
  bufAppend(' ');
  p->factor_1->accept(this);
  bufAppend(' ');
  p->factor_2->accept(this);
  bufAppend(')');
}
void ShowAbsyn::visitMulExp(MulExp *p)
{
  bufAppend('(');
  bufAppend("MulExp");
  bufAppend(' ');
  p->factor_1->accept(this);
  bufAppend(' ');
  p->factor_2->accept(this);
  bufAppend(')');
}
void ShowAbsyn::visitDivExp(DivExp *p)
{
  bufAppend('(');
  bufAppend("DivExp");
  bufAppend(' ');
  p->factor_1->accept(this);
  bufAppend(' ');
  p->factor_2->accept(this);
  bufAppend(')');
}
void ShowAbsyn::visitModExp(ModExp *p)
{
  bufAppend('(');
  bufAppend("ModExp");
  bufAppend(' ');
  p->factor_1->accept(this);
  bufAppend(' ');
  p->factor_2->accept(this);
  bufAppend(')');
}
void ShowAbsyn::visitImmExp(ImmExp *p)
{
  bufAppend('(');
  bufAppend("ImmExp");
  bufAppend(' ');
  bufAppend('[');
  if (p->factor_)  p->factor_->accept(this);
  bufAppend(']');
  bufAppend(')');
}
void ShowAbsyn::visitFactor(Factor *p) {} //abstract class

void ShowAbsyn::visitNumberFactor(NumberFactor *p)
{
  bufAppend('(');
  bufAppend("NumberFactor");
  bufAppend(' ');
  visitInteger(p->integer_);
  bufAppend(')');
}
void ShowAbsyn::visitHexFactor(HexFactor *p)
{
  bufAppend('(');
  bufAppend("HexFactor");
  bufAppend(' ');
  visitHex(p->hex_);
  bufAppend(')');
}
void ShowAbsyn::visitIdentFactor(IdentFactor *p)
{
  bufAppend('(');
  bufAppend("IdentFactor");
  bufAppend(' ');
  visitIdent(p->ident_);
  bufAppend(')');
}
void ShowAbsyn::visitStringFactor(StringFactor *p)
{
  bufAppend('(');
  bufAppend("StringFactor");
  bufAppend(' ');
  visitString(p->string_);
  bufAppend(')');
}
void ShowAbsyn::visitConfigType(ConfigType *p) {} //abstract class

void ShowAbsyn::visitBitsConfig(BitsConfig *p)
{
  bufAppend("BitsConfig");
}
void ShowAbsyn::visitInstConfig(InstConfig *p)
{
  bufAppend("InstConfig");
}
void ShowAbsyn::visitOptiConfig(OptiConfig *p)
{
  bufAppend("OptiConfig");
}
void ShowAbsyn::visitFormConfig(FormConfig *p)
{
  bufAppend("FormConfig");
}
void ShowAbsyn::visitPaddConfig(PaddConfig *p)
{
  bufAppend("PaddConfig");
}
void ShowAbsyn::visitPadsConfig(PadsConfig *p)
{
  bufAppend("PadsConfig");
}
void ShowAbsyn::visitSectConfig(SectConfig *p)
{
  bufAppend("SectConfig");
}
void ShowAbsyn::visitAbsoConfig(AbsoConfig *p)
{
  bufAppend("AbsoConfig");
}
void ShowAbsyn::visitFileConfig(FileConfig *p)
{
  bufAppend("FileConfig");
}
void ShowAbsyn::visitOpcode(Opcode *p) {} //abstract class

void ShowAbsyn::visitOpcodesAAA(OpcodesAAA *p)
{
  bufAppend("OpcodesAAA");
}
void ShowAbsyn::visitOpcodesAAD(OpcodesAAD *p)
{
  bufAppend("OpcodesAAD");
}
void ShowAbsyn::visitOpcodesAAS(OpcodesAAS *p)
{
  bufAppend("OpcodesAAS");
}
void ShowAbsyn::visitOpcodesAAM(OpcodesAAM *p)
{
  bufAppend("OpcodesAAM");
}
void ShowAbsyn::visitOpcodesADC(OpcodesADC *p)
{
  bufAppend("OpcodesADC");
}
void ShowAbsyn::visitOpcodesADD(OpcodesADD *p)
{
  bufAppend("OpcodesADD");
}
void ShowAbsyn::visitOpcodesAND(OpcodesAND *p)
{
  bufAppend("OpcodesAND");
}
void ShowAbsyn::visitOpcodesALIGN(OpcodesALIGN *p)
{
  bufAppend("OpcodesALIGN");
}
void ShowAbsyn::visitOpcodesALIGNB(OpcodesALIGNB *p)
{
  bufAppend("OpcodesALIGNB");
}
void ShowAbsyn::visitOpcodesARPL(OpcodesARPL *p)
{
  bufAppend("OpcodesARPL");
}
void ShowAbsyn::visitOpcodesBOUND(OpcodesBOUND *p)
{
  bufAppend("OpcodesBOUND");
}
void ShowAbsyn::visitOpcodesBSF(OpcodesBSF *p)
{
  bufAppend("OpcodesBSF");
}
void ShowAbsyn::visitOpcodesBSR(OpcodesBSR *p)
{
  bufAppend("OpcodesBSR");
}
void ShowAbsyn::visitOpcodesBSWAP(OpcodesBSWAP *p)
{
  bufAppend("OpcodesBSWAP");
}
void ShowAbsyn::visitOpcodesBT(OpcodesBT *p)
{
  bufAppend("OpcodesBT");
}
void ShowAbsyn::visitOpcodesBTC(OpcodesBTC *p)
{
  bufAppend("OpcodesBTC");
}
void ShowAbsyn::visitOpcodesBTR(OpcodesBTR *p)
{
  bufAppend("OpcodesBTR");
}
void ShowAbsyn::visitOpcodesBTS(OpcodesBTS *p)
{
  bufAppend("OpcodesBTS");
}
void ShowAbsyn::visitOpcodesCALL(OpcodesCALL *p)
{
  bufAppend("OpcodesCALL");
}
void ShowAbsyn::visitOpcodesCBW(OpcodesCBW *p)
{
  bufAppend("OpcodesCBW");
}
void ShowAbsyn::visitOpcodesCDQ(OpcodesCDQ *p)
{
  bufAppend("OpcodesCDQ");
}
void ShowAbsyn::visitOpcodesCLC(OpcodesCLC *p)
{
  bufAppend("OpcodesCLC");
}
void ShowAbsyn::visitOpcodesCLD(OpcodesCLD *p)
{
  bufAppend("OpcodesCLD");
}
void ShowAbsyn::visitOpcodesCLI(OpcodesCLI *p)
{
  bufAppend("OpcodesCLI");
}
void ShowAbsyn::visitOpcodesCLTS(OpcodesCLTS *p)
{
  bufAppend("OpcodesCLTS");
}
void ShowAbsyn::visitOpcodesCMC(OpcodesCMC *p)
{
  bufAppend("OpcodesCMC");
}
void ShowAbsyn::visitOpcodesCMP(OpcodesCMP *p)
{
  bufAppend("OpcodesCMP");
}
void ShowAbsyn::visitOpcodesCMPSB(OpcodesCMPSB *p)
{
  bufAppend("OpcodesCMPSB");
}
void ShowAbsyn::visitOpcodesCMPSD(OpcodesCMPSD *p)
{
  bufAppend("OpcodesCMPSD");
}
void ShowAbsyn::visitOpcodesCMPSW(OpcodesCMPSW *p)
{
  bufAppend("OpcodesCMPSW");
}
void ShowAbsyn::visitOpcodesCMPXCHG(OpcodesCMPXCHG *p)
{
  bufAppend("OpcodesCMPXCHG");
}
void ShowAbsyn::visitOpcodesCPUID(OpcodesCPUID *p)
{
  bufAppend("OpcodesCPUID");
}
void ShowAbsyn::visitOpcodesCWD(OpcodesCWD *p)
{
  bufAppend("OpcodesCWD");
}
void ShowAbsyn::visitOpcodesCWDE(OpcodesCWDE *p)
{
  bufAppend("OpcodesCWDE");
}
void ShowAbsyn::visitOpcodesDAA(OpcodesDAA *p)
{
  bufAppend("OpcodesDAA");
}
void ShowAbsyn::visitOpcodesDAS(OpcodesDAS *p)
{
  bufAppend("OpcodesDAS");
}
void ShowAbsyn::visitOpcodesDB(OpcodesDB *p)
{
  bufAppend("OpcodesDB");
}
void ShowAbsyn::visitOpcodesDD(OpcodesDD *p)
{
  bufAppend("OpcodesDD");
}
void ShowAbsyn::visitOpcodesDEC(OpcodesDEC *p)
{
  bufAppend("OpcodesDEC");
}
void ShowAbsyn::visitOpcodesDIV(OpcodesDIV *p)
{
  bufAppend("OpcodesDIV");
}
void ShowAbsyn::visitOpcodesDQ(OpcodesDQ *p)
{
  bufAppend("OpcodesDQ");
}
void ShowAbsyn::visitOpcodesDT(OpcodesDT *p)
{
  bufAppend("OpcodesDT");
}
void ShowAbsyn::visitOpcodesDW(OpcodesDW *p)
{
  bufAppend("OpcodesDW");
}
void ShowAbsyn::visitOpcodesEND(OpcodesEND *p)
{
  bufAppend("OpcodesEND");
}
void ShowAbsyn::visitOpcodesENTER(OpcodesENTER *p)
{
  bufAppend("OpcodesENTER");
}
void ShowAbsyn::visitOpcodesEXTERN(OpcodesEXTERN *p)
{
  bufAppend("OpcodesEXTERN");
}
void ShowAbsyn::visitOpcodesF2XM1(OpcodesF2XM1 *p)
{
  bufAppend("OpcodesF2XM1");
}
void ShowAbsyn::visitOpcodesFABS(OpcodesFABS *p)
{
  bufAppend("OpcodesFABS");
}
void ShowAbsyn::visitOpcodesFADD(OpcodesFADD *p)
{
  bufAppend("OpcodesFADD");
}
void ShowAbsyn::visitOpcodesFADDP(OpcodesFADDP *p)
{
  bufAppend("OpcodesFADDP");
}
void ShowAbsyn::visitOpcodesFBLD(OpcodesFBLD *p)
{
  bufAppend("OpcodesFBLD");
}
void ShowAbsyn::visitOpcodesFBSTP(OpcodesFBSTP *p)
{
  bufAppend("OpcodesFBSTP");
}
void ShowAbsyn::visitOpcodesFCHS(OpcodesFCHS *p)
{
  bufAppend("OpcodesFCHS");
}
void ShowAbsyn::visitOpcodesFCLEX(OpcodesFCLEX *p)
{
  bufAppend("OpcodesFCLEX");
}
void ShowAbsyn::visitOpcodesFCOM(OpcodesFCOM *p)
{
  bufAppend("OpcodesFCOM");
}
void ShowAbsyn::visitOpcodesFCOMP(OpcodesFCOMP *p)
{
  bufAppend("OpcodesFCOMP");
}
void ShowAbsyn::visitOpcodesFCOMPP(OpcodesFCOMPP *p)
{
  bufAppend("OpcodesFCOMPP");
}
void ShowAbsyn::visitOpcodesFCOS(OpcodesFCOS *p)
{
  bufAppend("OpcodesFCOS");
}
void ShowAbsyn::visitOpcodesFDECSTP(OpcodesFDECSTP *p)
{
  bufAppend("OpcodesFDECSTP");
}
void ShowAbsyn::visitOpcodesFDISI(OpcodesFDISI *p)
{
  bufAppend("OpcodesFDISI");
}
void ShowAbsyn::visitOpcodesFDIV(OpcodesFDIV *p)
{
  bufAppend("OpcodesFDIV");
}
void ShowAbsyn::visitOpcodesFDIVP(OpcodesFDIVP *p)
{
  bufAppend("OpcodesFDIVP");
}
void ShowAbsyn::visitOpcodesFDIVR(OpcodesFDIVR *p)
{
  bufAppend("OpcodesFDIVR");
}
void ShowAbsyn::visitOpcodesFDIVRP(OpcodesFDIVRP *p)
{
  bufAppend("OpcodesFDIVRP");
}
void ShowAbsyn::visitOpcodesFENI(OpcodesFENI *p)
{
  bufAppend("OpcodesFENI");
}
void ShowAbsyn::visitOpcodesFFREE(OpcodesFFREE *p)
{
  bufAppend("OpcodesFFREE");
}
void ShowAbsyn::visitOpcodesFIADD(OpcodesFIADD *p)
{
  bufAppend("OpcodesFIADD");
}
void ShowAbsyn::visitOpcodesFICOM(OpcodesFICOM *p)
{
  bufAppend("OpcodesFICOM");
}
void ShowAbsyn::visitOpcodesFICOMP(OpcodesFICOMP *p)
{
  bufAppend("OpcodesFICOMP");
}
void ShowAbsyn::visitOpcodesFIDIV(OpcodesFIDIV *p)
{
  bufAppend("OpcodesFIDIV");
}
void ShowAbsyn::visitOpcodesFIDIVR(OpcodesFIDIVR *p)
{
  bufAppend("OpcodesFIDIVR");
}
void ShowAbsyn::visitOpcodesFILD(OpcodesFILD *p)
{
  bufAppend("OpcodesFILD");
}
void ShowAbsyn::visitOpcodesFIMUL(OpcodesFIMUL *p)
{
  bufAppend("OpcodesFIMUL");
}
void ShowAbsyn::visitOpcodesFINCSTP(OpcodesFINCSTP *p)
{
  bufAppend("OpcodesFINCSTP");
}
void ShowAbsyn::visitOpcodesFINIT(OpcodesFINIT *p)
{
  bufAppend("OpcodesFINIT");
}
void ShowAbsyn::visitOpcodesFIST(OpcodesFIST *p)
{
  bufAppend("OpcodesFIST");
}
void ShowAbsyn::visitOpcodesFISTP(OpcodesFISTP *p)
{
  bufAppend("OpcodesFISTP");
}
void ShowAbsyn::visitOpcodesFISUB(OpcodesFISUB *p)
{
  bufAppend("OpcodesFISUB");
}
void ShowAbsyn::visitOpcodesFISUBR(OpcodesFISUBR *p)
{
  bufAppend("OpcodesFISUBR");
}
void ShowAbsyn::visitOpcodesFLD(OpcodesFLD *p)
{
  bufAppend("OpcodesFLD");
}
void ShowAbsyn::visitOpcodesFLD1(OpcodesFLD1 *p)
{
  bufAppend("OpcodesFLD1");
}
void ShowAbsyn::visitOpcodesFLDCW(OpcodesFLDCW *p)
{
  bufAppend("OpcodesFLDCW");
}
void ShowAbsyn::visitOpcodesFLDENV(OpcodesFLDENV *p)
{
  bufAppend("OpcodesFLDENV");
}
void ShowAbsyn::visitOpcodesFLDL2E(OpcodesFLDL2E *p)
{
  bufAppend("OpcodesFLDL2E");
}
void ShowAbsyn::visitOpcodesFLDL2T(OpcodesFLDL2T *p)
{
  bufAppend("OpcodesFLDL2T");
}
void ShowAbsyn::visitOpcodesFLDLG2(OpcodesFLDLG2 *p)
{
  bufAppend("OpcodesFLDLG2");
}
void ShowAbsyn::visitOpcodesFLDLN2(OpcodesFLDLN2 *p)
{
  bufAppend("OpcodesFLDLN2");
}
void ShowAbsyn::visitOpcodesFLDPI(OpcodesFLDPI *p)
{
  bufAppend("OpcodesFLDPI");
}
void ShowAbsyn::visitOpcodesFLDZ(OpcodesFLDZ *p)
{
  bufAppend("OpcodesFLDZ");
}
void ShowAbsyn::visitOpcodesFMUL(OpcodesFMUL *p)
{
  bufAppend("OpcodesFMUL");
}
void ShowAbsyn::visitOpcodesFMULP(OpcodesFMULP *p)
{
  bufAppend("OpcodesFMULP");
}
void ShowAbsyn::visitOpcodesFNCLEX(OpcodesFNCLEX *p)
{
  bufAppend("OpcodesFNCLEX");
}
void ShowAbsyn::visitOpcodesFNDISI(OpcodesFNDISI *p)
{
  bufAppend("OpcodesFNDISI");
}
void ShowAbsyn::visitOpcodesFNENI(OpcodesFNENI *p)
{
  bufAppend("OpcodesFNENI");
}
void ShowAbsyn::visitOpcodesFNINIT(OpcodesFNINIT *p)
{
  bufAppend("OpcodesFNINIT");
}
void ShowAbsyn::visitOpcodesFNOP(OpcodesFNOP *p)
{
  bufAppend("OpcodesFNOP");
}
void ShowAbsyn::visitOpcodesFNSAVE(OpcodesFNSAVE *p)
{
  bufAppend("OpcodesFNSAVE");
}
void ShowAbsyn::visitOpcodesFNSTCW(OpcodesFNSTCW *p)
{
  bufAppend("OpcodesFNSTCW");
}
void ShowAbsyn::visitOpcodesFNSTENV(OpcodesFNSTENV *p)
{
  bufAppend("OpcodesFNSTENV");
}
void ShowAbsyn::visitOpcodesFNSTSW(OpcodesFNSTSW *p)
{
  bufAppend("OpcodesFNSTSW");
}
void ShowAbsyn::visitOpcodesFPATAN(OpcodesFPATAN *p)
{
  bufAppend("OpcodesFPATAN");
}
void ShowAbsyn::visitOpcodesFPTAN(OpcodesFPTAN *p)
{
  bufAppend("OpcodesFPTAN");
}
void ShowAbsyn::visitOpcodesFPREM(OpcodesFPREM *p)
{
  bufAppend("OpcodesFPREM");
}
void ShowAbsyn::visitOpcodesFPREM1(OpcodesFPREM1 *p)
{
  bufAppend("OpcodesFPREM1");
}
void ShowAbsyn::visitOpcodesFRNDINT(OpcodesFRNDINT *p)
{
  bufAppend("OpcodesFRNDINT");
}
void ShowAbsyn::visitOpcodesFRSTOR(OpcodesFRSTOR *p)
{
  bufAppend("OpcodesFRSTOR");
}
void ShowAbsyn::visitOpcodesFSAVE(OpcodesFSAVE *p)
{
  bufAppend("OpcodesFSAVE");
}
void ShowAbsyn::visitOpcodesFSCALE(OpcodesFSCALE *p)
{
  bufAppend("OpcodesFSCALE");
}
void ShowAbsyn::visitOpcodesFSETPM(OpcodesFSETPM *p)
{
  bufAppend("OpcodesFSETPM");
}
void ShowAbsyn::visitOpcodesFSIN(OpcodesFSIN *p)
{
  bufAppend("OpcodesFSIN");
}
void ShowAbsyn::visitOpcodesFSINCOS(OpcodesFSINCOS *p)
{
  bufAppend("OpcodesFSINCOS");
}
void ShowAbsyn::visitOpcodesFSQRT(OpcodesFSQRT *p)
{
  bufAppend("OpcodesFSQRT");
}
void ShowAbsyn::visitOpcodesFST(OpcodesFST *p)
{
  bufAppend("OpcodesFST");
}
void ShowAbsyn::visitOpcodesFSTCW(OpcodesFSTCW *p)
{
  bufAppend("OpcodesFSTCW");
}
void ShowAbsyn::visitOpcodesFSTENV(OpcodesFSTENV *p)
{
  bufAppend("OpcodesFSTENV");
}
void ShowAbsyn::visitOpcodesFSTP(OpcodesFSTP *p)
{
  bufAppend("OpcodesFSTP");
}
void ShowAbsyn::visitOpcodesFSTSW(OpcodesFSTSW *p)
{
  bufAppend("OpcodesFSTSW");
}
void ShowAbsyn::visitOpcodesFSUB(OpcodesFSUB *p)
{
  bufAppend("OpcodesFSUB");
}
void ShowAbsyn::visitOpcodesFSUBP(OpcodesFSUBP *p)
{
  bufAppend("OpcodesFSUBP");
}
void ShowAbsyn::visitOpcodesFSUBR(OpcodesFSUBR *p)
{
  bufAppend("OpcodesFSUBR");
}
void ShowAbsyn::visitOpcodesFSUBRP(OpcodesFSUBRP *p)
{
  bufAppend("OpcodesFSUBRP");
}
void ShowAbsyn::visitOpcodesFTST(OpcodesFTST *p)
{
  bufAppend("OpcodesFTST");
}
void ShowAbsyn::visitOpcodesFUCOM(OpcodesFUCOM *p)
{
  bufAppend("OpcodesFUCOM");
}
void ShowAbsyn::visitOpcodesFUCOMP(OpcodesFUCOMP *p)
{
  bufAppend("OpcodesFUCOMP");
}
void ShowAbsyn::visitOpcodesFUCOMPP(OpcodesFUCOMPP *p)
{
  bufAppend("OpcodesFUCOMPP");
}
void ShowAbsyn::visitOpcodesFXAM(OpcodesFXAM *p)
{
  bufAppend("OpcodesFXAM");
}
void ShowAbsyn::visitOpcodesFXCH(OpcodesFXCH *p)
{
  bufAppend("OpcodesFXCH");
}
void ShowAbsyn::visitOpcodesFXTRACT(OpcodesFXTRACT *p)
{
  bufAppend("OpcodesFXTRACT");
}
void ShowAbsyn::visitOpcodesFYL2X(OpcodesFYL2X *p)
{
  bufAppend("OpcodesFYL2X");
}
void ShowAbsyn::visitOpcodesFYL2XP1(OpcodesFYL2XP1 *p)
{
  bufAppend("OpcodesFYL2XP1");
}
void ShowAbsyn::visitOpcodesHLT(OpcodesHLT *p)
{
  bufAppend("OpcodesHLT");
}
void ShowAbsyn::visitOpcodesIDIV(OpcodesIDIV *p)
{
  bufAppend("OpcodesIDIV");
}
void ShowAbsyn::visitOpcodesIMUL(OpcodesIMUL *p)
{
  bufAppend("OpcodesIMUL");
}
void ShowAbsyn::visitOpcodesIN(OpcodesIN *p)
{
  bufAppend("OpcodesIN");
}
void ShowAbsyn::visitOpcodesINC(OpcodesINC *p)
{
  bufAppend("OpcodesINC");
}
void ShowAbsyn::visitOpcodesINCO(OpcodesINCO *p)
{
  bufAppend("OpcodesINCO");
}
void ShowAbsyn::visitOpcodesINSB(OpcodesINSB *p)
{
  bufAppend("OpcodesINSB");
}
void ShowAbsyn::visitOpcodesINSD(OpcodesINSD *p)
{
  bufAppend("OpcodesINSD");
}
void ShowAbsyn::visitOpcodesINSW(OpcodesINSW *p)
{
  bufAppend("OpcodesINSW");
}
void ShowAbsyn::visitOpcodesINT(OpcodesINT *p)
{
  bufAppend("OpcodesINT");
}
void ShowAbsyn::visitOpcodesINT3(OpcodesINT3 *p)
{
  bufAppend("OpcodesINT3");
}
void ShowAbsyn::visitOpcodesINTO(OpcodesINTO *p)
{
  bufAppend("OpcodesINTO");
}
void ShowAbsyn::visitOpcodesINVD(OpcodesINVD *p)
{
  bufAppend("OpcodesINVD");
}
void ShowAbsyn::visitOpcodesINVLPG(OpcodesINVLPG *p)
{
  bufAppend("OpcodesINVLPG");
}
void ShowAbsyn::visitOpcodesIRET(OpcodesIRET *p)
{
  bufAppend("OpcodesIRET");
}
void ShowAbsyn::visitOpcodesIRETD(OpcodesIRETD *p)
{
  bufAppend("OpcodesIRETD");
}
void ShowAbsyn::visitOpcodesIRETW(OpcodesIRETW *p)
{
  bufAppend("OpcodesIRETW");
}
void ShowAbsyn::visitOpcodesJA(OpcodesJA *p)
{
  bufAppend("OpcodesJA");
}
void ShowAbsyn::visitOpcodesJAE(OpcodesJAE *p)
{
  bufAppend("OpcodesJAE");
}
void ShowAbsyn::visitOpcodesJB(OpcodesJB *p)
{
  bufAppend("OpcodesJB");
}
void ShowAbsyn::visitOpcodesJBE(OpcodesJBE *p)
{
  bufAppend("OpcodesJBE");
}
void ShowAbsyn::visitOpcodesJC(OpcodesJC *p)
{
  bufAppend("OpcodesJC");
}
void ShowAbsyn::visitOpcodesJCXZ(OpcodesJCXZ *p)
{
  bufAppend("OpcodesJCXZ");
}
void ShowAbsyn::visitOpcodesJE(OpcodesJE *p)
{
  bufAppend("OpcodesJE");
}
void ShowAbsyn::visitOpcodesJECXZ(OpcodesJECXZ *p)
{
  bufAppend("OpcodesJECXZ");
}
void ShowAbsyn::visitOpcodesJG(OpcodesJG *p)
{
  bufAppend("OpcodesJG");
}
void ShowAbsyn::visitOpcodesJGE(OpcodesJGE *p)
{
  bufAppend("OpcodesJGE");
}
void ShowAbsyn::visitOpcodesJL(OpcodesJL *p)
{
  bufAppend("OpcodesJL");
}
void ShowAbsyn::visitOpcodesJLE(OpcodesJLE *p)
{
  bufAppend("OpcodesJLE");
}
void ShowAbsyn::visitOpcodesJMP(OpcodesJMP *p)
{
  bufAppend("OpcodesJMP");
}
void ShowAbsyn::visitOpcodesJNA(OpcodesJNA *p)
{
  bufAppend("OpcodesJNA");
}
void ShowAbsyn::visitOpcodesJNAE(OpcodesJNAE *p)
{
  bufAppend("OpcodesJNAE");
}
void ShowAbsyn::visitOpcodesJNB(OpcodesJNB *p)
{
  bufAppend("OpcodesJNB");
}
void ShowAbsyn::visitOpcodesJNBE(OpcodesJNBE *p)
{
  bufAppend("OpcodesJNBE");
}
void ShowAbsyn::visitOpcodesJNC(OpcodesJNC *p)
{
  bufAppend("OpcodesJNC");
}
void ShowAbsyn::visitOpcodesJNE(OpcodesJNE *p)
{
  bufAppend("OpcodesJNE");
}
void ShowAbsyn::visitOpcodesJNG(OpcodesJNG *p)
{
  bufAppend("OpcodesJNG");
}
void ShowAbsyn::visitOpcodesJNGE(OpcodesJNGE *p)
{
  bufAppend("OpcodesJNGE");
}
void ShowAbsyn::visitOpcodesJNL(OpcodesJNL *p)
{
  bufAppend("OpcodesJNL");
}
void ShowAbsyn::visitOpcodesJNLE(OpcodesJNLE *p)
{
  bufAppend("OpcodesJNLE");
}
void ShowAbsyn::visitOpcodesJNO(OpcodesJNO *p)
{
  bufAppend("OpcodesJNO");
}
void ShowAbsyn::visitOpcodesJNP(OpcodesJNP *p)
{
  bufAppend("OpcodesJNP");
}
void ShowAbsyn::visitOpcodesJNS(OpcodesJNS *p)
{
  bufAppend("OpcodesJNS");
}
void ShowAbsyn::visitOpcodesJNZ(OpcodesJNZ *p)
{
  bufAppend("OpcodesJNZ");
}
void ShowAbsyn::visitOpcodesJO(OpcodesJO *p)
{
  bufAppend("OpcodesJO");
}
void ShowAbsyn::visitOpcodesJP(OpcodesJP *p)
{
  bufAppend("OpcodesJP");
}
void ShowAbsyn::visitOpcodesJPE(OpcodesJPE *p)
{
  bufAppend("OpcodesJPE");
}
void ShowAbsyn::visitOpcodesJPO(OpcodesJPO *p)
{
  bufAppend("OpcodesJPO");
}
void ShowAbsyn::visitOpcodesJS(OpcodesJS *p)
{
  bufAppend("OpcodesJS");
}
void ShowAbsyn::visitOpcodesJZ(OpcodesJZ *p)
{
  bufAppend("OpcodesJZ");
}
void ShowAbsyn::visitOpcodesLAHF(OpcodesLAHF *p)
{
  bufAppend("OpcodesLAHF");
}
void ShowAbsyn::visitOpcodesLAR(OpcodesLAR *p)
{
  bufAppend("OpcodesLAR");
}
void ShowAbsyn::visitOpcodesLDS(OpcodesLDS *p)
{
  bufAppend("OpcodesLDS");
}
void ShowAbsyn::visitOpcodesLEA(OpcodesLEA *p)
{
  bufAppend("OpcodesLEA");
}
void ShowAbsyn::visitOpcodesLEAVE(OpcodesLEAVE *p)
{
  bufAppend("OpcodesLEAVE");
}
void ShowAbsyn::visitOpcodesLES(OpcodesLES *p)
{
  bufAppend("OpcodesLES");
}
void ShowAbsyn::visitOpcodesLFS(OpcodesLFS *p)
{
  bufAppend("OpcodesLFS");
}
void ShowAbsyn::visitOpcodesLGDT(OpcodesLGDT *p)
{
  bufAppend("OpcodesLGDT");
}
void ShowAbsyn::visitOpcodesLGS(OpcodesLGS *p)
{
  bufAppend("OpcodesLGS");
}
void ShowAbsyn::visitOpcodesLIDT(OpcodesLIDT *p)
{
  bufAppend("OpcodesLIDT");
}
void ShowAbsyn::visitOpcodesLLDT(OpcodesLLDT *p)
{
  bufAppend("OpcodesLLDT");
}
void ShowAbsyn::visitOpcodesLMSW(OpcodesLMSW *p)
{
  bufAppend("OpcodesLMSW");
}
void ShowAbsyn::visitOpcodesLOCK(OpcodesLOCK *p)
{
  bufAppend("OpcodesLOCK");
}
void ShowAbsyn::visitOpcodesLODSB(OpcodesLODSB *p)
{
  bufAppend("OpcodesLODSB");
}
void ShowAbsyn::visitOpcodesLODSD(OpcodesLODSD *p)
{
  bufAppend("OpcodesLODSD");
}
void ShowAbsyn::visitOpcodesLODSW(OpcodesLODSW *p)
{
  bufAppend("OpcodesLODSW");
}
void ShowAbsyn::visitOpcodesLOOP(OpcodesLOOP *p)
{
  bufAppend("OpcodesLOOP");
}
void ShowAbsyn::visitOpcodesLOOPE(OpcodesLOOPE *p)
{
  bufAppend("OpcodesLOOPE");
}
void ShowAbsyn::visitOpcodesLOOPNE(OpcodesLOOPNE *p)
{
  bufAppend("OpcodesLOOPNE");
}
void ShowAbsyn::visitOpcodesLOOPNZ(OpcodesLOOPNZ *p)
{
  bufAppend("OpcodesLOOPNZ");
}
void ShowAbsyn::visitOpcodesLOOPZ(OpcodesLOOPZ *p)
{
  bufAppend("OpcodesLOOPZ");
}
void ShowAbsyn::visitOpcodesLSL(OpcodesLSL *p)
{
  bufAppend("OpcodesLSL");
}
void ShowAbsyn::visitOpcodesLSS(OpcodesLSS *p)
{
  bufAppend("OpcodesLSS");
}
void ShowAbsyn::visitOpcodesLTR(OpcodesLTR *p)
{
  bufAppend("OpcodesLTR");
}
void ShowAbsyn::visitOpcodesMOV(OpcodesMOV *p)
{
  bufAppend("OpcodesMOV");
}
void ShowAbsyn::visitOpcodesMOVSB(OpcodesMOVSB *p)
{
  bufAppend("OpcodesMOVSB");
}
void ShowAbsyn::visitOpcodesMOVSD(OpcodesMOVSD *p)
{
  bufAppend("OpcodesMOVSD");
}
void ShowAbsyn::visitOpcodesMOVSW(OpcodesMOVSW *p)
{
  bufAppend("OpcodesMOVSW");
}
void ShowAbsyn::visitOpcodesMOVSX(OpcodesMOVSX *p)
{
  bufAppend("OpcodesMOVSX");
}
void ShowAbsyn::visitOpcodesMOVZX(OpcodesMOVZX *p)
{
  bufAppend("OpcodesMOVZX");
}
void ShowAbsyn::visitOpcodesMUL(OpcodesMUL *p)
{
  bufAppend("OpcodesMUL");
}
void ShowAbsyn::visitOpcodesNEG(OpcodesNEG *p)
{
  bufAppend("OpcodesNEG");
}
void ShowAbsyn::visitOpcodesNOP(OpcodesNOP *p)
{
  bufAppend("OpcodesNOP");
}
void ShowAbsyn::visitOpcodesNOT(OpcodesNOT *p)
{
  bufAppend("OpcodesNOT");
}
void ShowAbsyn::visitOpcodesOR(OpcodesOR *p)
{
  bufAppend("OpcodesOR");
}
void ShowAbsyn::visitOpcodesORG(OpcodesORG *p)
{
  bufAppend("OpcodesORG");
}
void ShowAbsyn::visitOpcodesOUT(OpcodesOUT *p)
{
  bufAppend("OpcodesOUT");
}
void ShowAbsyn::visitOpcodesOUTSB(OpcodesOUTSB *p)
{
  bufAppend("OpcodesOUTSB");
}
void ShowAbsyn::visitOpcodesOUTSD(OpcodesOUTSD *p)
{
  bufAppend("OpcodesOUTSD");
}
void ShowAbsyn::visitOpcodesOUTSW(OpcodesOUTSW *p)
{
  bufAppend("OpcodesOUTSW");
}
void ShowAbsyn::visitOpcodesPOP(OpcodesPOP *p)
{
  bufAppend("OpcodesPOP");
}
void ShowAbsyn::visitOpcodesPOPA(OpcodesPOPA *p)
{
  bufAppend("OpcodesPOPA");
}
void ShowAbsyn::visitOpcodesPOPAD(OpcodesPOPAD *p)
{
  bufAppend("OpcodesPOPAD");
}
void ShowAbsyn::visitOpcodesPOPAW(OpcodesPOPAW *p)
{
  bufAppend("OpcodesPOPAW");
}
void ShowAbsyn::visitOpcodesPOPF(OpcodesPOPF *p)
{
  bufAppend("OpcodesPOPF");
}
void ShowAbsyn::visitOpcodesPOPFD(OpcodesPOPFD *p)
{
  bufAppend("OpcodesPOPFD");
}
void ShowAbsyn::visitOpcodesPOPFW(OpcodesPOPFW *p)
{
  bufAppend("OpcodesPOPFW");
}
void ShowAbsyn::visitOpcodesPUSH(OpcodesPUSH *p)
{
  bufAppend("OpcodesPUSH");
}
void ShowAbsyn::visitOpcodesPUSHA(OpcodesPUSHA *p)
{
  bufAppend("OpcodesPUSHA");
}
void ShowAbsyn::visitOpcodesPUSHD(OpcodesPUSHD *p)
{
  bufAppend("OpcodesPUSHD");
}
void ShowAbsyn::visitOpcodesPUSHAD(OpcodesPUSHAD *p)
{
  bufAppend("OpcodesPUSHAD");
}
void ShowAbsyn::visitOpcodesPUSHAW(OpcodesPUSHAW *p)
{
  bufAppend("OpcodesPUSHAW");
}
void ShowAbsyn::visitOpcodesPUSHF(OpcodesPUSHF *p)
{
  bufAppend("OpcodesPUSHF");
}
void ShowAbsyn::visitOpcodesPUSHFD(OpcodesPUSHFD *p)
{
  bufAppend("OpcodesPUSHFD");
}
void ShowAbsyn::visitOpcodesPUSHFW(OpcodesPUSHFW *p)
{
  bufAppend("OpcodesPUSHFW");
}
void ShowAbsyn::visitOpcodesRCL(OpcodesRCL *p)
{
  bufAppend("OpcodesRCL");
}
void ShowAbsyn::visitOpcodesRCR(OpcodesRCR *p)
{
  bufAppend("OpcodesRCR");
}
void ShowAbsyn::visitOpcodesRDMSR(OpcodesRDMSR *p)
{
  bufAppend("OpcodesRDMSR");
}
void ShowAbsyn::visitOpcodesRDPMC(OpcodesRDPMC *p)
{
  bufAppend("OpcodesRDPMC");
}
void ShowAbsyn::visitOpcodesREP(OpcodesREP *p)
{
  bufAppend("OpcodesREP");
}
void ShowAbsyn::visitOpcodesREPE(OpcodesREPE *p)
{
  bufAppend("OpcodesREPE");
}
void ShowAbsyn::visitOpcodesREPNE(OpcodesREPNE *p)
{
  bufAppend("OpcodesREPNE");
}
void ShowAbsyn::visitOpcodesREPNZ(OpcodesREPNZ *p)
{
  bufAppend("OpcodesREPNZ");
}
void ShowAbsyn::visitOpcodesREPZ(OpcodesREPZ *p)
{
  bufAppend("OpcodesREPZ");
}
void ShowAbsyn::visitOpcodesRESB(OpcodesRESB *p)
{
  bufAppend("OpcodesRESB");
}
void ShowAbsyn::visitOpcodesRESD(OpcodesRESD *p)
{
  bufAppend("OpcodesRESD");
}
void ShowAbsyn::visitOpcodesRESQ(OpcodesRESQ *p)
{
  bufAppend("OpcodesRESQ");
}
void ShowAbsyn::visitOpcodesREST(OpcodesREST *p)
{
  bufAppend("OpcodesREST");
}
void ShowAbsyn::visitOpcodesRESW(OpcodesRESW *p)
{
  bufAppend("OpcodesRESW");
}
void ShowAbsyn::visitOpcodesRET(OpcodesRET *p)
{
  bufAppend("OpcodesRET");
}
void ShowAbsyn::visitOpcodesRETF(OpcodesRETF *p)
{
  bufAppend("OpcodesRETF");
}
void ShowAbsyn::visitOpcodesRETN(OpcodesRETN *p)
{
  bufAppend("OpcodesRETN");
}
void ShowAbsyn::visitOpcodesROL(OpcodesROL *p)
{
  bufAppend("OpcodesROL");
}
void ShowAbsyn::visitOpcodesROR(OpcodesROR *p)
{
  bufAppend("OpcodesROR");
}
void ShowAbsyn::visitOpcodesRSM(OpcodesRSM *p)
{
  bufAppend("OpcodesRSM");
}
void ShowAbsyn::visitOpcodesSAHF(OpcodesSAHF *p)
{
  bufAppend("OpcodesSAHF");
}
void ShowAbsyn::visitOpcodesSAL(OpcodesSAL *p)
{
  bufAppend("OpcodesSAL");
}
void ShowAbsyn::visitOpcodesSAR(OpcodesSAR *p)
{
  bufAppend("OpcodesSAR");
}
void ShowAbsyn::visitOpcodesSBB(OpcodesSBB *p)
{
  bufAppend("OpcodesSBB");
}
void ShowAbsyn::visitOpcodesSCASB(OpcodesSCASB *p)
{
  bufAppend("OpcodesSCASB");
}
void ShowAbsyn::visitOpcodesSCASD(OpcodesSCASD *p)
{
  bufAppend("OpcodesSCASD");
}
void ShowAbsyn::visitOpcodesSCASW(OpcodesSCASW *p)
{
  bufAppend("OpcodesSCASW");
}
void ShowAbsyn::visitOpcodesSETA(OpcodesSETA *p)
{
  bufAppend("OpcodesSETA");
}
void ShowAbsyn::visitOpcodesSETAE(OpcodesSETAE *p)
{
  bufAppend("OpcodesSETAE");
}
void ShowAbsyn::visitOpcodesSETB(OpcodesSETB *p)
{
  bufAppend("OpcodesSETB");
}
void ShowAbsyn::visitOpcodesSETBE(OpcodesSETBE *p)
{
  bufAppend("OpcodesSETBE");
}
void ShowAbsyn::visitOpcodesSETC(OpcodesSETC *p)
{
  bufAppend("OpcodesSETC");
}
void ShowAbsyn::visitOpcodesSETE(OpcodesSETE *p)
{
  bufAppend("OpcodesSETE");
}
void ShowAbsyn::visitOpcodesSETG(OpcodesSETG *p)
{
  bufAppend("OpcodesSETG");
}
void ShowAbsyn::visitOpcodesSETGE(OpcodesSETGE *p)
{
  bufAppend("OpcodesSETGE");
}
void ShowAbsyn::visitOpcodesSETL(OpcodesSETL *p)
{
  bufAppend("OpcodesSETL");
}
void ShowAbsyn::visitOpcodesSETLE(OpcodesSETLE *p)
{
  bufAppend("OpcodesSETLE");
}
void ShowAbsyn::visitOpcodesSETNA(OpcodesSETNA *p)
{
  bufAppend("OpcodesSETNA");
}
void ShowAbsyn::visitOpcodesSETNAE(OpcodesSETNAE *p)
{
  bufAppend("OpcodesSETNAE");
}
void ShowAbsyn::visitOpcodesSETNB(OpcodesSETNB *p)
{
  bufAppend("OpcodesSETNB");
}
void ShowAbsyn::visitOpcodesSETNBE(OpcodesSETNBE *p)
{
  bufAppend("OpcodesSETNBE");
}
void ShowAbsyn::visitOpcodesSETNC(OpcodesSETNC *p)
{
  bufAppend("OpcodesSETNC");
}
void ShowAbsyn::visitOpcodesSETNE(OpcodesSETNE *p)
{
  bufAppend("OpcodesSETNE");
}
void ShowAbsyn::visitOpcodesSETNG(OpcodesSETNG *p)
{
  bufAppend("OpcodesSETNG");
}
void ShowAbsyn::visitOpcodesSETNGE(OpcodesSETNGE *p)
{
  bufAppend("OpcodesSETNGE");
}
void ShowAbsyn::visitOpcodesSETNL(OpcodesSETNL *p)
{
  bufAppend("OpcodesSETNL");
}
void ShowAbsyn::visitOpcodesSETNLE(OpcodesSETNLE *p)
{
  bufAppend("OpcodesSETNLE");
}
void ShowAbsyn::visitOpcodesSETNO(OpcodesSETNO *p)
{
  bufAppend("OpcodesSETNO");
}
void ShowAbsyn::visitOpcodesSETNP(OpcodesSETNP *p)
{
  bufAppend("OpcodesSETNP");
}
void ShowAbsyn::visitOpcodesSETNS(OpcodesSETNS *p)
{
  bufAppend("OpcodesSETNS");
}
void ShowAbsyn::visitOpcodesSETNZ(OpcodesSETNZ *p)
{
  bufAppend("OpcodesSETNZ");
}
void ShowAbsyn::visitOpcodesSETO(OpcodesSETO *p)
{
  bufAppend("OpcodesSETO");
}
void ShowAbsyn::visitOpcodesSETP(OpcodesSETP *p)
{
  bufAppend("OpcodesSETP");
}
void ShowAbsyn::visitOpcodesSETPE(OpcodesSETPE *p)
{
  bufAppend("OpcodesSETPE");
}
void ShowAbsyn::visitOpcodesSETPO(OpcodesSETPO *p)
{
  bufAppend("OpcodesSETPO");
}
void ShowAbsyn::visitOpcodesSETS(OpcodesSETS *p)
{
  bufAppend("OpcodesSETS");
}
void ShowAbsyn::visitOpcodesSETZ(OpcodesSETZ *p)
{
  bufAppend("OpcodesSETZ");
}
void ShowAbsyn::visitOpcodesSGDT(OpcodesSGDT *p)
{
  bufAppend("OpcodesSGDT");
}
void ShowAbsyn::visitOpcodesSHL(OpcodesSHL *p)
{
  bufAppend("OpcodesSHL");
}
void ShowAbsyn::visitOpcodesSHLD(OpcodesSHLD *p)
{
  bufAppend("OpcodesSHLD");
}
void ShowAbsyn::visitOpcodesSHR(OpcodesSHR *p)
{
  bufAppend("OpcodesSHR");
}
void ShowAbsyn::visitOpcodesSHRD(OpcodesSHRD *p)
{
  bufAppend("OpcodesSHRD");
}
void ShowAbsyn::visitOpcodesSIDT(OpcodesSIDT *p)
{
  bufAppend("OpcodesSIDT");
}
void ShowAbsyn::visitOpcodesSLDT(OpcodesSLDT *p)
{
  bufAppend("OpcodesSLDT");
}
void ShowAbsyn::visitOpcodesSMSW(OpcodesSMSW *p)
{
  bufAppend("OpcodesSMSW");
}
void ShowAbsyn::visitOpcodesSTC(OpcodesSTC *p)
{
  bufAppend("OpcodesSTC");
}
void ShowAbsyn::visitOpcodesSTD(OpcodesSTD *p)
{
  bufAppend("OpcodesSTD");
}
void ShowAbsyn::visitOpcodesSTI(OpcodesSTI *p)
{
  bufAppend("OpcodesSTI");
}
void ShowAbsyn::visitOpcodesSTOSB(OpcodesSTOSB *p)
{
  bufAppend("OpcodesSTOSB");
}
void ShowAbsyn::visitOpcodesSTOSD(OpcodesSTOSD *p)
{
  bufAppend("OpcodesSTOSD");
}
void ShowAbsyn::visitOpcodesSTOSW(OpcodesSTOSW *p)
{
  bufAppend("OpcodesSTOSW");
}
void ShowAbsyn::visitOpcodesSTR(OpcodesSTR *p)
{
  bufAppend("OpcodesSTR");
}
void ShowAbsyn::visitOpcodesSUB(OpcodesSUB *p)
{
  bufAppend("OpcodesSUB");
}
void ShowAbsyn::visitOpcodesTEST(OpcodesTEST *p)
{
  bufAppend("OpcodesTEST");
}
void ShowAbsyn::visitOpcodesTIMES(OpcodesTIMES *p)
{
  bufAppend("OpcodesTIMES");
}
void ShowAbsyn::visitOpcodesUD2(OpcodesUD2 *p)
{
  bufAppend("OpcodesUD2");
}
void ShowAbsyn::visitOpcodesVERR(OpcodesVERR *p)
{
  bufAppend("OpcodesVERR");
}
void ShowAbsyn::visitOpcodesVERW(OpcodesVERW *p)
{
  bufAppend("OpcodesVERW");
}
void ShowAbsyn::visitOpcodesWAIT(OpcodesWAIT *p)
{
  bufAppend("OpcodesWAIT");
}
void ShowAbsyn::visitOpcodesWBINVD(OpcodesWBINVD *p)
{
  bufAppend("OpcodesWBINVD");
}
void ShowAbsyn::visitOpcodesWRMSR(OpcodesWRMSR *p)
{
  bufAppend("OpcodesWRMSR");
}
void ShowAbsyn::visitOpcodesXADD(OpcodesXADD *p)
{
  bufAppend("OpcodesXADD");
}
void ShowAbsyn::visitOpcodesXCHG(OpcodesXCHG *p)
{
  bufAppend("OpcodesXCHG");
}
void ShowAbsyn::visitOpcodesXLATB(OpcodesXLATB *p)
{
  bufAppend("OpcodesXLATB");
}
void ShowAbsyn::visitOpcodesXOR(OpcodesXOR *p)
{
  bufAppend("OpcodesXOR");
}
void ShowAbsyn::visitInteger(Integer i)
{
  char tmp[20];
  sprintf(tmp, "%d", i);
  bufAppend(tmp);
}
void ShowAbsyn::visitDouble(Double d)
{
  char tmp[24];
  sprintf(tmp, "%.15g", d);
  bufAppend(tmp);
}
void ShowAbsyn::visitChar(Char c)
{
  bufAppend('\'');
  bufAppend(c);
  bufAppend('\'');
}
void ShowAbsyn::visitString(String s)
{
  bufAppend('\"');
  bufAppend(s);
  bufAppend('\"');
}
void ShowAbsyn::visitIdent(String s)
{
  bufAppend('\"');
  bufAppend(s);
  bufAppend('\"');
}

void ShowAbsyn::visitHex(String s)
{
  bufAppend('\"');
  bufAppend(s);
  bufAppend('\"');
}
