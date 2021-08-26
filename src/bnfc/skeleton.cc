/*** BNFC-Generated Visitor Design Pattern Skeleton. ***/
/* This implements the common visitor design pattern.
   Note that this method uses Visitor-traversal of lists, so
   List->accept() does NOT traverse the list. This allows different
   algorithms to use context information differently. */

#include "Skeleton.H"



void Skeleton::visitProgram(Program *t) {} //abstract class
void Skeleton::visitStatement(Statement *t) {} //abstract class
void Skeleton::visitMnemonicArgs(MnemonicArgs *t) {} //abstract class
void Skeleton::visitExp(Exp *t) {} //abstract class
void Skeleton::visitFactor(Factor *t) {} //abstract class
void Skeleton::visitConfigType(ConfigType *t) {} //abstract class
void Skeleton::visitOpcode(Opcode *t) {} //abstract class

void Skeleton::visitProg(Prog *prog)
{
  /* Code For Prog Goes Here */

  if (prog->liststatement_) prog->liststatement_->accept(this);

}

void Skeleton::visitLabelStmt(LabelStmt *label_stmt)
{
  /* Code For LabelStmt Goes Here */

  visitIdent(label_stmt->ident_);

}

void Skeleton::visitDeclareStmt(DeclareStmt *declare_stmt)
{
  /* Code For DeclareStmt Goes Here */

  visitIdent(declare_stmt->ident_);
  if (declare_stmt->exp_) declare_stmt->exp_->accept(this);

}

void Skeleton::visitConfigStmt(ConfigStmt *config_stmt)
{
  /* Code For ConfigStmt Goes Here */

  if (config_stmt->configtype_) config_stmt->configtype_->accept(this);
  visitString(config_stmt->string_);

}

void Skeleton::visitMnemonicStmt(MnemonicStmt *mnemonic_stmt)
{
  /* Code For MnemonicStmt Goes Here */

  if (mnemonic_stmt->opcode_) mnemonic_stmt->opcode_->accept(this);
  if (mnemonic_stmt->listmnemonicargs_) mnemonic_stmt->listmnemonicargs_->accept(this);

}

void Skeleton::visitMnemoArgs(MnemoArgs *mnemo_args)
{
  /* Code For MnemoArgs Goes Here */

  if (mnemo_args->exp_) mnemo_args->exp_->accept(this);

}

void Skeleton::visitEqExp(EqExp *eq_exp)
{
  /* Code For EqExp Goes Here */

  if (eq_exp->factor_1) eq_exp->factor_1->accept(this);
  if (eq_exp->factor_2) eq_exp->factor_2->accept(this);

}

void Skeleton::visitNeqExp(NeqExp *neq_exp)
{
  /* Code For NeqExp Goes Here */

  if (neq_exp->factor_1) neq_exp->factor_1->accept(this);
  if (neq_exp->factor_2) neq_exp->factor_2->accept(this);

}

void Skeleton::visitLtExp(LtExp *lt_exp)
{
  /* Code For LtExp Goes Here */

  if (lt_exp->factor_1) lt_exp->factor_1->accept(this);
  if (lt_exp->factor_2) lt_exp->factor_2->accept(this);

}

void Skeleton::visitGtExp(GtExp *gt_exp)
{
  /* Code For GtExp Goes Here */

  if (gt_exp->factor_1) gt_exp->factor_1->accept(this);
  if (gt_exp->factor_2) gt_exp->factor_2->accept(this);

}

void Skeleton::visitLteExp(LteExp *lte_exp)
{
  /* Code For LteExp Goes Here */

  if (lte_exp->factor_1) lte_exp->factor_1->accept(this);
  if (lte_exp->factor_2) lte_exp->factor_2->accept(this);

}

void Skeleton::visitGteExp(GteExp *gte_exp)
{
  /* Code For GteExp Goes Here */

  if (gte_exp->factor_1) gte_exp->factor_1->accept(this);
  if (gte_exp->factor_2) gte_exp->factor_2->accept(this);

}

void Skeleton::visitPlusExp(PlusExp *plus_exp)
{
  /* Code For PlusExp Goes Here */

  if (plus_exp->factor_1) plus_exp->factor_1->accept(this);
  if (plus_exp->factor_2) plus_exp->factor_2->accept(this);

}

void Skeleton::visitMinusExp(MinusExp *minus_exp)
{
  /* Code For MinusExp Goes Here */

  if (minus_exp->factor_1) minus_exp->factor_1->accept(this);
  if (minus_exp->factor_2) minus_exp->factor_2->accept(this);

}

void Skeleton::visitMulExp(MulExp *mul_exp)
{
  /* Code For MulExp Goes Here */

  if (mul_exp->factor_1) mul_exp->factor_1->accept(this);
  if (mul_exp->factor_2) mul_exp->factor_2->accept(this);

}

void Skeleton::visitDivExp(DivExp *div_exp)
{
  /* Code For DivExp Goes Here */

  if (div_exp->factor_1) div_exp->factor_1->accept(this);
  if (div_exp->factor_2) div_exp->factor_2->accept(this);

}

void Skeleton::visitModExp(ModExp *mod_exp)
{
  /* Code For ModExp Goes Here */

  if (mod_exp->factor_1) mod_exp->factor_1->accept(this);
  if (mod_exp->factor_2) mod_exp->factor_2->accept(this);

}

void Skeleton::visitImmExp(ImmExp *imm_exp)
{
  /* Code For ImmExp Goes Here */

  if (imm_exp->factor_) imm_exp->factor_->accept(this);

}

void Skeleton::visitNumberFactor(NumberFactor *number_factor)
{
  /* Code For NumberFactor Goes Here */

  visitInteger(number_factor->integer_);

}

void Skeleton::visitHexFactor(HexFactor *hex_factor)
{
  /* Code For HexFactor Goes Here */

  visitHex(hex_factor->hex_);

}

void Skeleton::visitIdentFactor(IdentFactor *ident_factor)
{
  /* Code For IdentFactor Goes Here */

  visitIdent(ident_factor->ident_);

}

void Skeleton::visitStringFactor(StringFactor *string_factor)
{
  /* Code For StringFactor Goes Here */

  visitString(string_factor->string_);

}

void Skeleton::visitBitsConfig(BitsConfig *bits_config)
{
  /* Code For BitsConfig Goes Here */


}

void Skeleton::visitInstConfig(InstConfig *inst_config)
{
  /* Code For InstConfig Goes Here */


}

void Skeleton::visitOptiConfig(OptiConfig *opti_config)
{
  /* Code For OptiConfig Goes Here */


}

void Skeleton::visitFormConfig(FormConfig *form_config)
{
  /* Code For FormConfig Goes Here */


}

void Skeleton::visitPaddConfig(PaddConfig *padd_config)
{
  /* Code For PaddConfig Goes Here */


}

void Skeleton::visitPadsConfig(PadsConfig *pads_config)
{
  /* Code For PadsConfig Goes Here */


}

void Skeleton::visitSectConfig(SectConfig *sect_config)
{
  /* Code For SectConfig Goes Here */


}

void Skeleton::visitAbsoConfig(AbsoConfig *abso_config)
{
  /* Code For AbsoConfig Goes Here */


}

void Skeleton::visitFileConfig(FileConfig *file_config)
{
  /* Code For FileConfig Goes Here */


}

void Skeleton::visitOpcodesAAA(OpcodesAAA *opcodes_aaa)
{
  /* Code For OpcodesAAA Goes Here */


}

void Skeleton::visitOpcodesAAD(OpcodesAAD *opcodes_aad)
{
  /* Code For OpcodesAAD Goes Here */


}

void Skeleton::visitOpcodesAAS(OpcodesAAS *opcodes_aas)
{
  /* Code For OpcodesAAS Goes Here */


}

void Skeleton::visitOpcodesAAM(OpcodesAAM *opcodes_aam)
{
  /* Code For OpcodesAAM Goes Here */


}

void Skeleton::visitOpcodesADC(OpcodesADC *opcodes_adc)
{
  /* Code For OpcodesADC Goes Here */


}

void Skeleton::visitOpcodesADD(OpcodesADD *opcodes_add)
{
  /* Code For OpcodesADD Goes Here */


}

void Skeleton::visitOpcodesAND(OpcodesAND *opcodes_and)
{
  /* Code For OpcodesAND Goes Here */


}

void Skeleton::visitOpcodesALIGN(OpcodesALIGN *opcodes_align)
{
  /* Code For OpcodesALIGN Goes Here */


}

void Skeleton::visitOpcodesALIGNB(OpcodesALIGNB *opcodes_alignb)
{
  /* Code For OpcodesALIGNB Goes Here */


}

void Skeleton::visitOpcodesARPL(OpcodesARPL *opcodes_arpl)
{
  /* Code For OpcodesARPL Goes Here */


}

void Skeleton::visitOpcodesBOUND(OpcodesBOUND *opcodes_bound)
{
  /* Code For OpcodesBOUND Goes Here */


}

void Skeleton::visitOpcodesBSF(OpcodesBSF *opcodes_bsf)
{
  /* Code For OpcodesBSF Goes Here */


}

void Skeleton::visitOpcodesBSR(OpcodesBSR *opcodes_bsr)
{
  /* Code For OpcodesBSR Goes Here */


}

void Skeleton::visitOpcodesBSWAP(OpcodesBSWAP *opcodes_bswap)
{
  /* Code For OpcodesBSWAP Goes Here */


}

void Skeleton::visitOpcodesBT(OpcodesBT *opcodes_bt)
{
  /* Code For OpcodesBT Goes Here */


}

void Skeleton::visitOpcodesBTC(OpcodesBTC *opcodes_btc)
{
  /* Code For OpcodesBTC Goes Here */


}

void Skeleton::visitOpcodesBTR(OpcodesBTR *opcodes_btr)
{
  /* Code For OpcodesBTR Goes Here */


}

void Skeleton::visitOpcodesBTS(OpcodesBTS *opcodes_bts)
{
  /* Code For OpcodesBTS Goes Here */


}

void Skeleton::visitOpcodesCALL(OpcodesCALL *opcodes_call)
{
  /* Code For OpcodesCALL Goes Here */


}

void Skeleton::visitOpcodesCBW(OpcodesCBW *opcodes_cbw)
{
  /* Code For OpcodesCBW Goes Here */


}

void Skeleton::visitOpcodesCDQ(OpcodesCDQ *opcodes_cdq)
{
  /* Code For OpcodesCDQ Goes Here */


}

void Skeleton::visitOpcodesCLC(OpcodesCLC *opcodes_clc)
{
  /* Code For OpcodesCLC Goes Here */


}

void Skeleton::visitOpcodesCLD(OpcodesCLD *opcodes_cld)
{
  /* Code For OpcodesCLD Goes Here */


}

void Skeleton::visitOpcodesCLI(OpcodesCLI *opcodes_cli)
{
  /* Code For OpcodesCLI Goes Here */


}

void Skeleton::visitOpcodesCLTS(OpcodesCLTS *opcodes_clts)
{
  /* Code For OpcodesCLTS Goes Here */


}

void Skeleton::visitOpcodesCMC(OpcodesCMC *opcodes_cmc)
{
  /* Code For OpcodesCMC Goes Here */


}

void Skeleton::visitOpcodesCMP(OpcodesCMP *opcodes_cmp)
{
  /* Code For OpcodesCMP Goes Here */


}

void Skeleton::visitOpcodesCMPSB(OpcodesCMPSB *opcodes_cmpsb)
{
  /* Code For OpcodesCMPSB Goes Here */


}

void Skeleton::visitOpcodesCMPSD(OpcodesCMPSD *opcodes_cmpsd)
{
  /* Code For OpcodesCMPSD Goes Here */


}

void Skeleton::visitOpcodesCMPSW(OpcodesCMPSW *opcodes_cmpsw)
{
  /* Code For OpcodesCMPSW Goes Here */


}

void Skeleton::visitOpcodesCMPXCHG(OpcodesCMPXCHG *opcodes_cmpxchg)
{
  /* Code For OpcodesCMPXCHG Goes Here */


}

void Skeleton::visitOpcodesCPUID(OpcodesCPUID *opcodes_cpuid)
{
  /* Code For OpcodesCPUID Goes Here */


}

void Skeleton::visitOpcodesCWD(OpcodesCWD *opcodes_cwd)
{
  /* Code For OpcodesCWD Goes Here */


}

void Skeleton::visitOpcodesCWDE(OpcodesCWDE *opcodes_cwde)
{
  /* Code For OpcodesCWDE Goes Here */


}

void Skeleton::visitOpcodesDAA(OpcodesDAA *opcodes_daa)
{
  /* Code For OpcodesDAA Goes Here */


}

void Skeleton::visitOpcodesDAS(OpcodesDAS *opcodes_das)
{
  /* Code For OpcodesDAS Goes Here */


}

void Skeleton::visitOpcodesDB(OpcodesDB *opcodes_db)
{
  /* Code For OpcodesDB Goes Here */


}

void Skeleton::visitOpcodesDD(OpcodesDD *opcodes_dd)
{
  /* Code For OpcodesDD Goes Here */


}

void Skeleton::visitOpcodesDEC(OpcodesDEC *opcodes_dec)
{
  /* Code For OpcodesDEC Goes Here */


}

void Skeleton::visitOpcodesDIV(OpcodesDIV *opcodes_div)
{
  /* Code For OpcodesDIV Goes Here */


}

void Skeleton::visitOpcodesDQ(OpcodesDQ *opcodes_dq)
{
  /* Code For OpcodesDQ Goes Here */


}

void Skeleton::visitOpcodesDT(OpcodesDT *opcodes_dt)
{
  /* Code For OpcodesDT Goes Here */


}

void Skeleton::visitOpcodesDW(OpcodesDW *opcodes_dw)
{
  /* Code For OpcodesDW Goes Here */


}

void Skeleton::visitOpcodesEND(OpcodesEND *opcodes_end)
{
  /* Code For OpcodesEND Goes Here */


}

void Skeleton::visitOpcodesENTER(OpcodesENTER *opcodes_enter)
{
  /* Code For OpcodesENTER Goes Here */


}

void Skeleton::visitOpcodesEXTERN(OpcodesEXTERN *opcodes_extern)
{
  /* Code For OpcodesEXTERN Goes Here */


}

void Skeleton::visitOpcodesF2XM1(OpcodesF2XM1 *opcodes_f_xm)
{
  /* Code For OpcodesF2XM1 Goes Here */


}

void Skeleton::visitOpcodesFABS(OpcodesFABS *opcodes_fabs)
{
  /* Code For OpcodesFABS Goes Here */


}

void Skeleton::visitOpcodesFADD(OpcodesFADD *opcodes_fadd)
{
  /* Code For OpcodesFADD Goes Here */


}

void Skeleton::visitOpcodesFADDP(OpcodesFADDP *opcodes_faddp)
{
  /* Code For OpcodesFADDP Goes Here */


}

void Skeleton::visitOpcodesFBLD(OpcodesFBLD *opcodes_fbld)
{
  /* Code For OpcodesFBLD Goes Here */


}

void Skeleton::visitOpcodesFBSTP(OpcodesFBSTP *opcodes_fbstp)
{
  /* Code For OpcodesFBSTP Goes Here */


}

void Skeleton::visitOpcodesFCHS(OpcodesFCHS *opcodes_fchs)
{
  /* Code For OpcodesFCHS Goes Here */


}

void Skeleton::visitOpcodesFCLEX(OpcodesFCLEX *opcodes_fclex)
{
  /* Code For OpcodesFCLEX Goes Here */


}

void Skeleton::visitOpcodesFCOM(OpcodesFCOM *opcodes_fcom)
{
  /* Code For OpcodesFCOM Goes Here */


}

void Skeleton::visitOpcodesFCOMP(OpcodesFCOMP *opcodes_fcomp)
{
  /* Code For OpcodesFCOMP Goes Here */


}

void Skeleton::visitOpcodesFCOMPP(OpcodesFCOMPP *opcodes_fcompp)
{
  /* Code For OpcodesFCOMPP Goes Here */


}

void Skeleton::visitOpcodesFCOS(OpcodesFCOS *opcodes_fcos)
{
  /* Code For OpcodesFCOS Goes Here */


}

void Skeleton::visitOpcodesFDECSTP(OpcodesFDECSTP *opcodes_fdecstp)
{
  /* Code For OpcodesFDECSTP Goes Here */


}

void Skeleton::visitOpcodesFDISI(OpcodesFDISI *opcodes_fdisi)
{
  /* Code For OpcodesFDISI Goes Here */


}

void Skeleton::visitOpcodesFDIV(OpcodesFDIV *opcodes_fdiv)
{
  /* Code For OpcodesFDIV Goes Here */


}

void Skeleton::visitOpcodesFDIVP(OpcodesFDIVP *opcodes_fdivp)
{
  /* Code For OpcodesFDIVP Goes Here */


}

void Skeleton::visitOpcodesFDIVR(OpcodesFDIVR *opcodes_fdivr)
{
  /* Code For OpcodesFDIVR Goes Here */


}

void Skeleton::visitOpcodesFDIVRP(OpcodesFDIVRP *opcodes_fdivrp)
{
  /* Code For OpcodesFDIVRP Goes Here */


}

void Skeleton::visitOpcodesFENI(OpcodesFENI *opcodes_feni)
{
  /* Code For OpcodesFENI Goes Here */


}

void Skeleton::visitOpcodesFFREE(OpcodesFFREE *opcodes_ffree)
{
  /* Code For OpcodesFFREE Goes Here */


}

void Skeleton::visitOpcodesFIADD(OpcodesFIADD *opcodes_fiadd)
{
  /* Code For OpcodesFIADD Goes Here */


}

void Skeleton::visitOpcodesFICOM(OpcodesFICOM *opcodes_ficom)
{
  /* Code For OpcodesFICOM Goes Here */


}

void Skeleton::visitOpcodesFICOMP(OpcodesFICOMP *opcodes_ficomp)
{
  /* Code For OpcodesFICOMP Goes Here */


}

void Skeleton::visitOpcodesFIDIV(OpcodesFIDIV *opcodes_fidiv)
{
  /* Code For OpcodesFIDIV Goes Here */


}

void Skeleton::visitOpcodesFIDIVR(OpcodesFIDIVR *opcodes_fidivr)
{
  /* Code For OpcodesFIDIVR Goes Here */


}

void Skeleton::visitOpcodesFILD(OpcodesFILD *opcodes_fild)
{
  /* Code For OpcodesFILD Goes Here */


}

void Skeleton::visitOpcodesFIMUL(OpcodesFIMUL *opcodes_fimul)
{
  /* Code For OpcodesFIMUL Goes Here */


}

void Skeleton::visitOpcodesFINCSTP(OpcodesFINCSTP *opcodes_fincstp)
{
  /* Code For OpcodesFINCSTP Goes Here */


}

void Skeleton::visitOpcodesFINIT(OpcodesFINIT *opcodes_finit)
{
  /* Code For OpcodesFINIT Goes Here */


}

void Skeleton::visitOpcodesFIST(OpcodesFIST *opcodes_fist)
{
  /* Code For OpcodesFIST Goes Here */


}

void Skeleton::visitOpcodesFISTP(OpcodesFISTP *opcodes_fistp)
{
  /* Code For OpcodesFISTP Goes Here */


}

void Skeleton::visitOpcodesFISUB(OpcodesFISUB *opcodes_fisub)
{
  /* Code For OpcodesFISUB Goes Here */


}

void Skeleton::visitOpcodesFISUBR(OpcodesFISUBR *opcodes_fisubr)
{
  /* Code For OpcodesFISUBR Goes Here */


}

void Skeleton::visitOpcodesFLD(OpcodesFLD *opcodes_fld)
{
  /* Code For OpcodesFLD Goes Here */


}

void Skeleton::visitOpcodesFLD1(OpcodesFLD1 *opcodes_fld)
{
  /* Code For OpcodesFLD1 Goes Here */


}

void Skeleton::visitOpcodesFLDCW(OpcodesFLDCW *opcodes_fldcw)
{
  /* Code For OpcodesFLDCW Goes Here */


}

void Skeleton::visitOpcodesFLDENV(OpcodesFLDENV *opcodes_fldenv)
{
  /* Code For OpcodesFLDENV Goes Here */


}

void Skeleton::visitOpcodesFLDL2E(OpcodesFLDL2E *opcodes_fldl_e)
{
  /* Code For OpcodesFLDL2E Goes Here */


}

void Skeleton::visitOpcodesFLDL2T(OpcodesFLDL2T *opcodes_fldl_t)
{
  /* Code For OpcodesFLDL2T Goes Here */


}

void Skeleton::visitOpcodesFLDLG2(OpcodesFLDLG2 *opcodes_fldlg)
{
  /* Code For OpcodesFLDLG2 Goes Here */


}

void Skeleton::visitOpcodesFLDLN2(OpcodesFLDLN2 *opcodes_fldln)
{
  /* Code For OpcodesFLDLN2 Goes Here */


}

void Skeleton::visitOpcodesFLDPI(OpcodesFLDPI *opcodes_fldpi)
{
  /* Code For OpcodesFLDPI Goes Here */


}

void Skeleton::visitOpcodesFLDZ(OpcodesFLDZ *opcodes_fldz)
{
  /* Code For OpcodesFLDZ Goes Here */


}

void Skeleton::visitOpcodesFMUL(OpcodesFMUL *opcodes_fmul)
{
  /* Code For OpcodesFMUL Goes Here */


}

void Skeleton::visitOpcodesFMULP(OpcodesFMULP *opcodes_fmulp)
{
  /* Code For OpcodesFMULP Goes Here */


}

void Skeleton::visitOpcodesFNCLEX(OpcodesFNCLEX *opcodes_fnclex)
{
  /* Code For OpcodesFNCLEX Goes Here */


}

void Skeleton::visitOpcodesFNDISI(OpcodesFNDISI *opcodes_fndisi)
{
  /* Code For OpcodesFNDISI Goes Here */


}

void Skeleton::visitOpcodesFNENI(OpcodesFNENI *opcodes_fneni)
{
  /* Code For OpcodesFNENI Goes Here */


}

void Skeleton::visitOpcodesFNINIT(OpcodesFNINIT *opcodes_fninit)
{
  /* Code For OpcodesFNINIT Goes Here */


}

void Skeleton::visitOpcodesFNOP(OpcodesFNOP *opcodes_fnop)
{
  /* Code For OpcodesFNOP Goes Here */


}

void Skeleton::visitOpcodesFNSAVE(OpcodesFNSAVE *opcodes_fnsave)
{
  /* Code For OpcodesFNSAVE Goes Here */


}

void Skeleton::visitOpcodesFNSTCW(OpcodesFNSTCW *opcodes_fnstcw)
{
  /* Code For OpcodesFNSTCW Goes Here */


}

void Skeleton::visitOpcodesFNSTENV(OpcodesFNSTENV *opcodes_fnstenv)
{
  /* Code For OpcodesFNSTENV Goes Here */


}

void Skeleton::visitOpcodesFNSTSW(OpcodesFNSTSW *opcodes_fnstsw)
{
  /* Code For OpcodesFNSTSW Goes Here */


}

void Skeleton::visitOpcodesFPATAN(OpcodesFPATAN *opcodes_fpatan)
{
  /* Code For OpcodesFPATAN Goes Here */


}

void Skeleton::visitOpcodesFPTAN(OpcodesFPTAN *opcodes_fptan)
{
  /* Code For OpcodesFPTAN Goes Here */


}

void Skeleton::visitOpcodesFPREM(OpcodesFPREM *opcodes_fprem)
{
  /* Code For OpcodesFPREM Goes Here */


}

void Skeleton::visitOpcodesFPREM1(OpcodesFPREM1 *opcodes_fprem)
{
  /* Code For OpcodesFPREM1 Goes Here */


}

void Skeleton::visitOpcodesFRNDINT(OpcodesFRNDINT *opcodes_frndint)
{
  /* Code For OpcodesFRNDINT Goes Here */


}

void Skeleton::visitOpcodesFRSTOR(OpcodesFRSTOR *opcodes_frstor)
{
  /* Code For OpcodesFRSTOR Goes Here */


}

void Skeleton::visitOpcodesFSAVE(OpcodesFSAVE *opcodes_fsave)
{
  /* Code For OpcodesFSAVE Goes Here */


}

void Skeleton::visitOpcodesFSCALE(OpcodesFSCALE *opcodes_fscale)
{
  /* Code For OpcodesFSCALE Goes Here */


}

void Skeleton::visitOpcodesFSETPM(OpcodesFSETPM *opcodes_fsetpm)
{
  /* Code For OpcodesFSETPM Goes Here */


}

void Skeleton::visitOpcodesFSIN(OpcodesFSIN *opcodes_fsin)
{
  /* Code For OpcodesFSIN Goes Here */


}

void Skeleton::visitOpcodesFSINCOS(OpcodesFSINCOS *opcodes_fsincos)
{
  /* Code For OpcodesFSINCOS Goes Here */


}

void Skeleton::visitOpcodesFSQRT(OpcodesFSQRT *opcodes_fsqrt)
{
  /* Code For OpcodesFSQRT Goes Here */


}

void Skeleton::visitOpcodesFST(OpcodesFST *opcodes_fst)
{
  /* Code For OpcodesFST Goes Here */


}

void Skeleton::visitOpcodesFSTCW(OpcodesFSTCW *opcodes_fstcw)
{
  /* Code For OpcodesFSTCW Goes Here */


}

void Skeleton::visitOpcodesFSTENV(OpcodesFSTENV *opcodes_fstenv)
{
  /* Code For OpcodesFSTENV Goes Here */


}

void Skeleton::visitOpcodesFSTP(OpcodesFSTP *opcodes_fstp)
{
  /* Code For OpcodesFSTP Goes Here */


}

void Skeleton::visitOpcodesFSTSW(OpcodesFSTSW *opcodes_fstsw)
{
  /* Code For OpcodesFSTSW Goes Here */


}

void Skeleton::visitOpcodesFSUB(OpcodesFSUB *opcodes_fsub)
{
  /* Code For OpcodesFSUB Goes Here */


}

void Skeleton::visitOpcodesFSUBP(OpcodesFSUBP *opcodes_fsubp)
{
  /* Code For OpcodesFSUBP Goes Here */


}

void Skeleton::visitOpcodesFSUBR(OpcodesFSUBR *opcodes_fsubr)
{
  /* Code For OpcodesFSUBR Goes Here */


}

void Skeleton::visitOpcodesFSUBRP(OpcodesFSUBRP *opcodes_fsubrp)
{
  /* Code For OpcodesFSUBRP Goes Here */


}

void Skeleton::visitOpcodesFTST(OpcodesFTST *opcodes_ftst)
{
  /* Code For OpcodesFTST Goes Here */


}

void Skeleton::visitOpcodesFUCOM(OpcodesFUCOM *opcodes_fucom)
{
  /* Code For OpcodesFUCOM Goes Here */


}

void Skeleton::visitOpcodesFUCOMP(OpcodesFUCOMP *opcodes_fucomp)
{
  /* Code For OpcodesFUCOMP Goes Here */


}

void Skeleton::visitOpcodesFUCOMPP(OpcodesFUCOMPP *opcodes_fucompp)
{
  /* Code For OpcodesFUCOMPP Goes Here */


}

void Skeleton::visitOpcodesFXAM(OpcodesFXAM *opcodes_fxam)
{
  /* Code For OpcodesFXAM Goes Here */


}

void Skeleton::visitOpcodesFXCH(OpcodesFXCH *opcodes_fxch)
{
  /* Code For OpcodesFXCH Goes Here */


}

void Skeleton::visitOpcodesFXTRACT(OpcodesFXTRACT *opcodes_fxtract)
{
  /* Code For OpcodesFXTRACT Goes Here */


}

void Skeleton::visitOpcodesFYL2X(OpcodesFYL2X *opcodes_fyl_x)
{
  /* Code For OpcodesFYL2X Goes Here */


}

void Skeleton::visitOpcodesFYL2XP1(OpcodesFYL2XP1 *opcodes_fyl_xp)
{
  /* Code For OpcodesFYL2XP1 Goes Here */


}

void Skeleton::visitOpcodesHLT(OpcodesHLT *opcodes_hlt)
{
  /* Code For OpcodesHLT Goes Here */


}

void Skeleton::visitOpcodesIDIV(OpcodesIDIV *opcodes_idiv)
{
  /* Code For OpcodesIDIV Goes Here */


}

void Skeleton::visitOpcodesIMUL(OpcodesIMUL *opcodes_imul)
{
  /* Code For OpcodesIMUL Goes Here */


}

void Skeleton::visitOpcodesIN(OpcodesIN *opcodes_in)
{
  /* Code For OpcodesIN Goes Here */


}

void Skeleton::visitOpcodesINC(OpcodesINC *opcodes_inc)
{
  /* Code For OpcodesINC Goes Here */


}

void Skeleton::visitOpcodesINCO(OpcodesINCO *opcodes_inco)
{
  /* Code For OpcodesINCO Goes Here */


}

void Skeleton::visitOpcodesINSB(OpcodesINSB *opcodes_insb)
{
  /* Code For OpcodesINSB Goes Here */


}

void Skeleton::visitOpcodesINSD(OpcodesINSD *opcodes_insd)
{
  /* Code For OpcodesINSD Goes Here */


}

void Skeleton::visitOpcodesINSW(OpcodesINSW *opcodes_insw)
{
  /* Code For OpcodesINSW Goes Here */


}

void Skeleton::visitOpcodesINT(OpcodesINT *opcodes_int)
{
  /* Code For OpcodesINT Goes Here */


}

void Skeleton::visitOpcodesINT3(OpcodesINT3 *opcodes_int)
{
  /* Code For OpcodesINT3 Goes Here */


}

void Skeleton::visitOpcodesINTO(OpcodesINTO *opcodes_into)
{
  /* Code For OpcodesINTO Goes Here */


}

void Skeleton::visitOpcodesINVD(OpcodesINVD *opcodes_invd)
{
  /* Code For OpcodesINVD Goes Here */


}

void Skeleton::visitOpcodesINVLPG(OpcodesINVLPG *opcodes_invlpg)
{
  /* Code For OpcodesINVLPG Goes Here */


}

void Skeleton::visitOpcodesIRET(OpcodesIRET *opcodes_iret)
{
  /* Code For OpcodesIRET Goes Here */


}

void Skeleton::visitOpcodesIRETD(OpcodesIRETD *opcodes_iretd)
{
  /* Code For OpcodesIRETD Goes Here */


}

void Skeleton::visitOpcodesIRETW(OpcodesIRETW *opcodes_iretw)
{
  /* Code For OpcodesIRETW Goes Here */


}

void Skeleton::visitOpcodesJA(OpcodesJA *opcodes_ja)
{
  /* Code For OpcodesJA Goes Here */


}

void Skeleton::visitOpcodesJAE(OpcodesJAE *opcodes_jae)
{
  /* Code For OpcodesJAE Goes Here */


}

void Skeleton::visitOpcodesJB(OpcodesJB *opcodes_jb)
{
  /* Code For OpcodesJB Goes Here */


}

void Skeleton::visitOpcodesJBE(OpcodesJBE *opcodes_jbe)
{
  /* Code For OpcodesJBE Goes Here */


}

void Skeleton::visitOpcodesJC(OpcodesJC *opcodes_jc)
{
  /* Code For OpcodesJC Goes Here */


}

void Skeleton::visitOpcodesJCXZ(OpcodesJCXZ *opcodes_jcxz)
{
  /* Code For OpcodesJCXZ Goes Here */


}

void Skeleton::visitOpcodesJE(OpcodesJE *opcodes_je)
{
  /* Code For OpcodesJE Goes Here */


}

void Skeleton::visitOpcodesJECXZ(OpcodesJECXZ *opcodes_jecxz)
{
  /* Code For OpcodesJECXZ Goes Here */


}

void Skeleton::visitOpcodesJG(OpcodesJG *opcodes_jg)
{
  /* Code For OpcodesJG Goes Here */


}

void Skeleton::visitOpcodesJGE(OpcodesJGE *opcodes_jge)
{
  /* Code For OpcodesJGE Goes Here */


}

void Skeleton::visitOpcodesJL(OpcodesJL *opcodes_jl)
{
  /* Code For OpcodesJL Goes Here */


}

void Skeleton::visitOpcodesJLE(OpcodesJLE *opcodes_jle)
{
  /* Code For OpcodesJLE Goes Here */


}

void Skeleton::visitOpcodesJMP(OpcodesJMP *opcodes_jmp)
{
  /* Code For OpcodesJMP Goes Here */


}

void Skeleton::visitOpcodesJNA(OpcodesJNA *opcodes_jna)
{
  /* Code For OpcodesJNA Goes Here */


}

void Skeleton::visitOpcodesJNAE(OpcodesJNAE *opcodes_jnae)
{
  /* Code For OpcodesJNAE Goes Here */


}

void Skeleton::visitOpcodesJNB(OpcodesJNB *opcodes_jnb)
{
  /* Code For OpcodesJNB Goes Here */


}

void Skeleton::visitOpcodesJNBE(OpcodesJNBE *opcodes_jnbe)
{
  /* Code For OpcodesJNBE Goes Here */


}

void Skeleton::visitOpcodesJNC(OpcodesJNC *opcodes_jnc)
{
  /* Code For OpcodesJNC Goes Here */


}

void Skeleton::visitOpcodesJNE(OpcodesJNE *opcodes_jne)
{
  /* Code For OpcodesJNE Goes Here */


}

void Skeleton::visitOpcodesJNG(OpcodesJNG *opcodes_jng)
{
  /* Code For OpcodesJNG Goes Here */


}

void Skeleton::visitOpcodesJNGE(OpcodesJNGE *opcodes_jnge)
{
  /* Code For OpcodesJNGE Goes Here */


}

void Skeleton::visitOpcodesJNL(OpcodesJNL *opcodes_jnl)
{
  /* Code For OpcodesJNL Goes Here */


}

void Skeleton::visitOpcodesJNLE(OpcodesJNLE *opcodes_jnle)
{
  /* Code For OpcodesJNLE Goes Here */


}

void Skeleton::visitOpcodesJNO(OpcodesJNO *opcodes_jno)
{
  /* Code For OpcodesJNO Goes Here */


}

void Skeleton::visitOpcodesJNP(OpcodesJNP *opcodes_jnp)
{
  /* Code For OpcodesJNP Goes Here */


}

void Skeleton::visitOpcodesJNS(OpcodesJNS *opcodes_jns)
{
  /* Code For OpcodesJNS Goes Here */


}

void Skeleton::visitOpcodesJNZ(OpcodesJNZ *opcodes_jnz)
{
  /* Code For OpcodesJNZ Goes Here */


}

void Skeleton::visitOpcodesJO(OpcodesJO *opcodes_jo)
{
  /* Code For OpcodesJO Goes Here */


}

void Skeleton::visitOpcodesJP(OpcodesJP *opcodes_jp)
{
  /* Code For OpcodesJP Goes Here */


}

void Skeleton::visitOpcodesJPE(OpcodesJPE *opcodes_jpe)
{
  /* Code For OpcodesJPE Goes Here */


}

void Skeleton::visitOpcodesJPO(OpcodesJPO *opcodes_jpo)
{
  /* Code For OpcodesJPO Goes Here */


}

void Skeleton::visitOpcodesJS(OpcodesJS *opcodes_js)
{
  /* Code For OpcodesJS Goes Here */


}

void Skeleton::visitOpcodesJZ(OpcodesJZ *opcodes_jz)
{
  /* Code For OpcodesJZ Goes Here */


}

void Skeleton::visitOpcodesLAHF(OpcodesLAHF *opcodes_lahf)
{
  /* Code For OpcodesLAHF Goes Here */


}

void Skeleton::visitOpcodesLAR(OpcodesLAR *opcodes_lar)
{
  /* Code For OpcodesLAR Goes Here */


}

void Skeleton::visitOpcodesLDS(OpcodesLDS *opcodes_lds)
{
  /* Code For OpcodesLDS Goes Here */


}

void Skeleton::visitOpcodesLEA(OpcodesLEA *opcodes_lea)
{
  /* Code For OpcodesLEA Goes Here */


}

void Skeleton::visitOpcodesLEAVE(OpcodesLEAVE *opcodes_leave)
{
  /* Code For OpcodesLEAVE Goes Here */


}

void Skeleton::visitOpcodesLES(OpcodesLES *opcodes_les)
{
  /* Code For OpcodesLES Goes Here */


}

void Skeleton::visitOpcodesLFS(OpcodesLFS *opcodes_lfs)
{
  /* Code For OpcodesLFS Goes Here */


}

void Skeleton::visitOpcodesLGDT(OpcodesLGDT *opcodes_lgdt)
{
  /* Code For OpcodesLGDT Goes Here */


}

void Skeleton::visitOpcodesLGS(OpcodesLGS *opcodes_lgs)
{
  /* Code For OpcodesLGS Goes Here */


}

void Skeleton::visitOpcodesLIDT(OpcodesLIDT *opcodes_lidt)
{
  /* Code For OpcodesLIDT Goes Here */


}

void Skeleton::visitOpcodesLLDT(OpcodesLLDT *opcodes_lldt)
{
  /* Code For OpcodesLLDT Goes Here */


}

void Skeleton::visitOpcodesLMSW(OpcodesLMSW *opcodes_lmsw)
{
  /* Code For OpcodesLMSW Goes Here */


}

void Skeleton::visitOpcodesLOCK(OpcodesLOCK *opcodes_lock)
{
  /* Code For OpcodesLOCK Goes Here */


}

void Skeleton::visitOpcodesLODSB(OpcodesLODSB *opcodes_lodsb)
{
  /* Code For OpcodesLODSB Goes Here */


}

void Skeleton::visitOpcodesLODSD(OpcodesLODSD *opcodes_lodsd)
{
  /* Code For OpcodesLODSD Goes Here */


}

void Skeleton::visitOpcodesLODSW(OpcodesLODSW *opcodes_lodsw)
{
  /* Code For OpcodesLODSW Goes Here */


}

void Skeleton::visitOpcodesLOOP(OpcodesLOOP *opcodes_loop)
{
  /* Code For OpcodesLOOP Goes Here */


}

void Skeleton::visitOpcodesLOOPE(OpcodesLOOPE *opcodes_loope)
{
  /* Code For OpcodesLOOPE Goes Here */


}

void Skeleton::visitOpcodesLOOPNE(OpcodesLOOPNE *opcodes_loopne)
{
  /* Code For OpcodesLOOPNE Goes Here */


}

void Skeleton::visitOpcodesLOOPNZ(OpcodesLOOPNZ *opcodes_loopnz)
{
  /* Code For OpcodesLOOPNZ Goes Here */


}

void Skeleton::visitOpcodesLOOPZ(OpcodesLOOPZ *opcodes_loopz)
{
  /* Code For OpcodesLOOPZ Goes Here */


}

void Skeleton::visitOpcodesLSL(OpcodesLSL *opcodes_lsl)
{
  /* Code For OpcodesLSL Goes Here */


}

void Skeleton::visitOpcodesLSS(OpcodesLSS *opcodes_lss)
{
  /* Code For OpcodesLSS Goes Here */


}

void Skeleton::visitOpcodesLTR(OpcodesLTR *opcodes_ltr)
{
  /* Code For OpcodesLTR Goes Here */


}

void Skeleton::visitOpcodesMOV(OpcodesMOV *opcodes_mov)
{
  /* Code For OpcodesMOV Goes Here */


}

void Skeleton::visitOpcodesMOVSB(OpcodesMOVSB *opcodes_movsb)
{
  /* Code For OpcodesMOVSB Goes Here */


}

void Skeleton::visitOpcodesMOVSD(OpcodesMOVSD *opcodes_movsd)
{
  /* Code For OpcodesMOVSD Goes Here */


}

void Skeleton::visitOpcodesMOVSW(OpcodesMOVSW *opcodes_movsw)
{
  /* Code For OpcodesMOVSW Goes Here */


}

void Skeleton::visitOpcodesMOVSX(OpcodesMOVSX *opcodes_movsx)
{
  /* Code For OpcodesMOVSX Goes Here */


}

void Skeleton::visitOpcodesMOVZX(OpcodesMOVZX *opcodes_movzx)
{
  /* Code For OpcodesMOVZX Goes Here */


}

void Skeleton::visitOpcodesMUL(OpcodesMUL *opcodes_mul)
{
  /* Code For OpcodesMUL Goes Here */


}

void Skeleton::visitOpcodesNEG(OpcodesNEG *opcodes_neg)
{
  /* Code For OpcodesNEG Goes Here */


}

void Skeleton::visitOpcodesNOP(OpcodesNOP *opcodes_nop)
{
  /* Code For OpcodesNOP Goes Here */


}

void Skeleton::visitOpcodesNOT(OpcodesNOT *opcodes_not)
{
  /* Code For OpcodesNOT Goes Here */


}

void Skeleton::visitOpcodesOR(OpcodesOR *opcodes_or)
{
  /* Code For OpcodesOR Goes Here */


}

void Skeleton::visitOpcodesORG(OpcodesORG *opcodes_org)
{
  /* Code For OpcodesORG Goes Here */


}

void Skeleton::visitOpcodesOUT(OpcodesOUT *opcodes_out)
{
  /* Code For OpcodesOUT Goes Here */


}

void Skeleton::visitOpcodesOUTSB(OpcodesOUTSB *opcodes_outsb)
{
  /* Code For OpcodesOUTSB Goes Here */


}

void Skeleton::visitOpcodesOUTSD(OpcodesOUTSD *opcodes_outsd)
{
  /* Code For OpcodesOUTSD Goes Here */


}

void Skeleton::visitOpcodesOUTSW(OpcodesOUTSW *opcodes_outsw)
{
  /* Code For OpcodesOUTSW Goes Here */


}

void Skeleton::visitOpcodesPOP(OpcodesPOP *opcodes_pop)
{
  /* Code For OpcodesPOP Goes Here */


}

void Skeleton::visitOpcodesPOPA(OpcodesPOPA *opcodes_popa)
{
  /* Code For OpcodesPOPA Goes Here */


}

void Skeleton::visitOpcodesPOPAD(OpcodesPOPAD *opcodes_popad)
{
  /* Code For OpcodesPOPAD Goes Here */


}

void Skeleton::visitOpcodesPOPAW(OpcodesPOPAW *opcodes_popaw)
{
  /* Code For OpcodesPOPAW Goes Here */


}

void Skeleton::visitOpcodesPOPF(OpcodesPOPF *opcodes_popf)
{
  /* Code For OpcodesPOPF Goes Here */


}

void Skeleton::visitOpcodesPOPFD(OpcodesPOPFD *opcodes_popfd)
{
  /* Code For OpcodesPOPFD Goes Here */


}

void Skeleton::visitOpcodesPOPFW(OpcodesPOPFW *opcodes_popfw)
{
  /* Code For OpcodesPOPFW Goes Here */


}

void Skeleton::visitOpcodesPUSH(OpcodesPUSH *opcodes_push)
{
  /* Code For OpcodesPUSH Goes Here */


}

void Skeleton::visitOpcodesPUSHA(OpcodesPUSHA *opcodes_pusha)
{
  /* Code For OpcodesPUSHA Goes Here */


}

void Skeleton::visitOpcodesPUSHD(OpcodesPUSHD *opcodes_pushd)
{
  /* Code For OpcodesPUSHD Goes Here */


}

void Skeleton::visitOpcodesPUSHAD(OpcodesPUSHAD *opcodes_pushad)
{
  /* Code For OpcodesPUSHAD Goes Here */


}

void Skeleton::visitOpcodesPUSHAW(OpcodesPUSHAW *opcodes_pushaw)
{
  /* Code For OpcodesPUSHAW Goes Here */


}

void Skeleton::visitOpcodesPUSHF(OpcodesPUSHF *opcodes_pushf)
{
  /* Code For OpcodesPUSHF Goes Here */


}

void Skeleton::visitOpcodesPUSHFD(OpcodesPUSHFD *opcodes_pushfd)
{
  /* Code For OpcodesPUSHFD Goes Here */


}

void Skeleton::visitOpcodesPUSHFW(OpcodesPUSHFW *opcodes_pushfw)
{
  /* Code For OpcodesPUSHFW Goes Here */


}

void Skeleton::visitOpcodesRCL(OpcodesRCL *opcodes_rcl)
{
  /* Code For OpcodesRCL Goes Here */


}

void Skeleton::visitOpcodesRCR(OpcodesRCR *opcodes_rcr)
{
  /* Code For OpcodesRCR Goes Here */


}

void Skeleton::visitOpcodesRDMSR(OpcodesRDMSR *opcodes_rdmsr)
{
  /* Code For OpcodesRDMSR Goes Here */


}

void Skeleton::visitOpcodesRDPMC(OpcodesRDPMC *opcodes_rdpmc)
{
  /* Code For OpcodesRDPMC Goes Here */


}

void Skeleton::visitOpcodesREP(OpcodesREP *opcodes_rep)
{
  /* Code For OpcodesREP Goes Here */


}

void Skeleton::visitOpcodesREPE(OpcodesREPE *opcodes_repe)
{
  /* Code For OpcodesREPE Goes Here */


}

void Skeleton::visitOpcodesREPNE(OpcodesREPNE *opcodes_repne)
{
  /* Code For OpcodesREPNE Goes Here */


}

void Skeleton::visitOpcodesREPNZ(OpcodesREPNZ *opcodes_repnz)
{
  /* Code For OpcodesREPNZ Goes Here */


}

void Skeleton::visitOpcodesREPZ(OpcodesREPZ *opcodes_repz)
{
  /* Code For OpcodesREPZ Goes Here */


}

void Skeleton::visitOpcodesRESB(OpcodesRESB *opcodes_resb)
{
  /* Code For OpcodesRESB Goes Here */


}

void Skeleton::visitOpcodesRESD(OpcodesRESD *opcodes_resd)
{
  /* Code For OpcodesRESD Goes Here */


}

void Skeleton::visitOpcodesRESQ(OpcodesRESQ *opcodes_resq)
{
  /* Code For OpcodesRESQ Goes Here */


}

void Skeleton::visitOpcodesREST(OpcodesREST *opcodes_rest)
{
  /* Code For OpcodesREST Goes Here */


}

void Skeleton::visitOpcodesRESW(OpcodesRESW *opcodes_resw)
{
  /* Code For OpcodesRESW Goes Here */


}

void Skeleton::visitOpcodesRET(OpcodesRET *opcodes_ret)
{
  /* Code For OpcodesRET Goes Here */


}

void Skeleton::visitOpcodesRETF(OpcodesRETF *opcodes_retf)
{
  /* Code For OpcodesRETF Goes Here */


}

void Skeleton::visitOpcodesRETN(OpcodesRETN *opcodes_retn)
{
  /* Code For OpcodesRETN Goes Here */


}

void Skeleton::visitOpcodesROL(OpcodesROL *opcodes_rol)
{
  /* Code For OpcodesROL Goes Here */


}

void Skeleton::visitOpcodesROR(OpcodesROR *opcodes_ror)
{
  /* Code For OpcodesROR Goes Here */


}

void Skeleton::visitOpcodesRSM(OpcodesRSM *opcodes_rsm)
{
  /* Code For OpcodesRSM Goes Here */


}

void Skeleton::visitOpcodesSAHF(OpcodesSAHF *opcodes_sahf)
{
  /* Code For OpcodesSAHF Goes Here */


}

void Skeleton::visitOpcodesSAL(OpcodesSAL *opcodes_sal)
{
  /* Code For OpcodesSAL Goes Here */


}

void Skeleton::visitOpcodesSAR(OpcodesSAR *opcodes_sar)
{
  /* Code For OpcodesSAR Goes Here */


}

void Skeleton::visitOpcodesSBB(OpcodesSBB *opcodes_sbb)
{
  /* Code For OpcodesSBB Goes Here */


}

void Skeleton::visitOpcodesSCASB(OpcodesSCASB *opcodes_scasb)
{
  /* Code For OpcodesSCASB Goes Here */


}

void Skeleton::visitOpcodesSCASD(OpcodesSCASD *opcodes_scasd)
{
  /* Code For OpcodesSCASD Goes Here */


}

void Skeleton::visitOpcodesSCASW(OpcodesSCASW *opcodes_scasw)
{
  /* Code For OpcodesSCASW Goes Here */


}

void Skeleton::visitOpcodesSETA(OpcodesSETA *opcodes_seta)
{
  /* Code For OpcodesSETA Goes Here */


}

void Skeleton::visitOpcodesSETAE(OpcodesSETAE *opcodes_setae)
{
  /* Code For OpcodesSETAE Goes Here */


}

void Skeleton::visitOpcodesSETB(OpcodesSETB *opcodes_setb)
{
  /* Code For OpcodesSETB Goes Here */


}

void Skeleton::visitOpcodesSETBE(OpcodesSETBE *opcodes_setbe)
{
  /* Code For OpcodesSETBE Goes Here */


}

void Skeleton::visitOpcodesSETC(OpcodesSETC *opcodes_setc)
{
  /* Code For OpcodesSETC Goes Here */


}

void Skeleton::visitOpcodesSETE(OpcodesSETE *opcodes_sete)
{
  /* Code For OpcodesSETE Goes Here */


}

void Skeleton::visitOpcodesSETG(OpcodesSETG *opcodes_setg)
{
  /* Code For OpcodesSETG Goes Here */


}

void Skeleton::visitOpcodesSETGE(OpcodesSETGE *opcodes_setge)
{
  /* Code For OpcodesSETGE Goes Here */


}

void Skeleton::visitOpcodesSETL(OpcodesSETL *opcodes_setl)
{
  /* Code For OpcodesSETL Goes Here */


}

void Skeleton::visitOpcodesSETLE(OpcodesSETLE *opcodes_setle)
{
  /* Code For OpcodesSETLE Goes Here */


}

void Skeleton::visitOpcodesSETNA(OpcodesSETNA *opcodes_setna)
{
  /* Code For OpcodesSETNA Goes Here */


}

void Skeleton::visitOpcodesSETNAE(OpcodesSETNAE *opcodes_setnae)
{
  /* Code For OpcodesSETNAE Goes Here */


}

void Skeleton::visitOpcodesSETNB(OpcodesSETNB *opcodes_setnb)
{
  /* Code For OpcodesSETNB Goes Here */


}

void Skeleton::visitOpcodesSETNBE(OpcodesSETNBE *opcodes_setnbe)
{
  /* Code For OpcodesSETNBE Goes Here */


}

void Skeleton::visitOpcodesSETNC(OpcodesSETNC *opcodes_setnc)
{
  /* Code For OpcodesSETNC Goes Here */


}

void Skeleton::visitOpcodesSETNE(OpcodesSETNE *opcodes_setne)
{
  /* Code For OpcodesSETNE Goes Here */


}

void Skeleton::visitOpcodesSETNG(OpcodesSETNG *opcodes_setng)
{
  /* Code For OpcodesSETNG Goes Here */


}

void Skeleton::visitOpcodesSETNGE(OpcodesSETNGE *opcodes_setnge)
{
  /* Code For OpcodesSETNGE Goes Here */


}

void Skeleton::visitOpcodesSETNL(OpcodesSETNL *opcodes_setnl)
{
  /* Code For OpcodesSETNL Goes Here */


}

void Skeleton::visitOpcodesSETNLE(OpcodesSETNLE *opcodes_setnle)
{
  /* Code For OpcodesSETNLE Goes Here */


}

void Skeleton::visitOpcodesSETNO(OpcodesSETNO *opcodes_setno)
{
  /* Code For OpcodesSETNO Goes Here */


}

void Skeleton::visitOpcodesSETNP(OpcodesSETNP *opcodes_setnp)
{
  /* Code For OpcodesSETNP Goes Here */


}

void Skeleton::visitOpcodesSETNS(OpcodesSETNS *opcodes_setns)
{
  /* Code For OpcodesSETNS Goes Here */


}

void Skeleton::visitOpcodesSETNZ(OpcodesSETNZ *opcodes_setnz)
{
  /* Code For OpcodesSETNZ Goes Here */


}

void Skeleton::visitOpcodesSETO(OpcodesSETO *opcodes_seto)
{
  /* Code For OpcodesSETO Goes Here */


}

void Skeleton::visitOpcodesSETP(OpcodesSETP *opcodes_setp)
{
  /* Code For OpcodesSETP Goes Here */


}

void Skeleton::visitOpcodesSETPE(OpcodesSETPE *opcodes_setpe)
{
  /* Code For OpcodesSETPE Goes Here */


}

void Skeleton::visitOpcodesSETPO(OpcodesSETPO *opcodes_setpo)
{
  /* Code For OpcodesSETPO Goes Here */


}

void Skeleton::visitOpcodesSETS(OpcodesSETS *opcodes_sets)
{
  /* Code For OpcodesSETS Goes Here */


}

void Skeleton::visitOpcodesSETZ(OpcodesSETZ *opcodes_setz)
{
  /* Code For OpcodesSETZ Goes Here */


}

void Skeleton::visitOpcodesSGDT(OpcodesSGDT *opcodes_sgdt)
{
  /* Code For OpcodesSGDT Goes Here */


}

void Skeleton::visitOpcodesSHL(OpcodesSHL *opcodes_shl)
{
  /* Code For OpcodesSHL Goes Here */


}

void Skeleton::visitOpcodesSHLD(OpcodesSHLD *opcodes_shld)
{
  /* Code For OpcodesSHLD Goes Here */


}

void Skeleton::visitOpcodesSHR(OpcodesSHR *opcodes_shr)
{
  /* Code For OpcodesSHR Goes Here */


}

void Skeleton::visitOpcodesSHRD(OpcodesSHRD *opcodes_shrd)
{
  /* Code For OpcodesSHRD Goes Here */


}

void Skeleton::visitOpcodesSIDT(OpcodesSIDT *opcodes_sidt)
{
  /* Code For OpcodesSIDT Goes Here */


}

void Skeleton::visitOpcodesSLDT(OpcodesSLDT *opcodes_sldt)
{
  /* Code For OpcodesSLDT Goes Here */


}

void Skeleton::visitOpcodesSMSW(OpcodesSMSW *opcodes_smsw)
{
  /* Code For OpcodesSMSW Goes Here */


}

void Skeleton::visitOpcodesSTC(OpcodesSTC *opcodes_stc)
{
  /* Code For OpcodesSTC Goes Here */


}

void Skeleton::visitOpcodesSTD(OpcodesSTD *opcodes_std)
{
  /* Code For OpcodesSTD Goes Here */


}

void Skeleton::visitOpcodesSTI(OpcodesSTI *opcodes_sti)
{
  /* Code For OpcodesSTI Goes Here */


}

void Skeleton::visitOpcodesSTOSB(OpcodesSTOSB *opcodes_stosb)
{
  /* Code For OpcodesSTOSB Goes Here */


}

void Skeleton::visitOpcodesSTOSD(OpcodesSTOSD *opcodes_stosd)
{
  /* Code For OpcodesSTOSD Goes Here */


}

void Skeleton::visitOpcodesSTOSW(OpcodesSTOSW *opcodes_stosw)
{
  /* Code For OpcodesSTOSW Goes Here */


}

void Skeleton::visitOpcodesSTR(OpcodesSTR *opcodes_str)
{
  /* Code For OpcodesSTR Goes Here */


}

void Skeleton::visitOpcodesSUB(OpcodesSUB *opcodes_sub)
{
  /* Code For OpcodesSUB Goes Here */


}

void Skeleton::visitOpcodesTEST(OpcodesTEST *opcodes_test)
{
  /* Code For OpcodesTEST Goes Here */


}

void Skeleton::visitOpcodesTIMES(OpcodesTIMES *opcodes_times)
{
  /* Code For OpcodesTIMES Goes Here */


}

void Skeleton::visitOpcodesUD2(OpcodesUD2 *opcodes_ud)
{
  /* Code For OpcodesUD2 Goes Here */


}

void Skeleton::visitOpcodesVERR(OpcodesVERR *opcodes_verr)
{
  /* Code For OpcodesVERR Goes Here */


}

void Skeleton::visitOpcodesVERW(OpcodesVERW *opcodes_verw)
{
  /* Code For OpcodesVERW Goes Here */


}

void Skeleton::visitOpcodesWAIT(OpcodesWAIT *opcodes_wait)
{
  /* Code For OpcodesWAIT Goes Here */


}

void Skeleton::visitOpcodesWBINVD(OpcodesWBINVD *opcodes_wbinvd)
{
  /* Code For OpcodesWBINVD Goes Here */


}

void Skeleton::visitOpcodesWRMSR(OpcodesWRMSR *opcodes_wrmsr)
{
  /* Code For OpcodesWRMSR Goes Here */


}

void Skeleton::visitOpcodesXADD(OpcodesXADD *opcodes_xadd)
{
  /* Code For OpcodesXADD Goes Here */


}

void Skeleton::visitOpcodesXCHG(OpcodesXCHG *opcodes_xchg)
{
  /* Code For OpcodesXCHG Goes Here */


}

void Skeleton::visitOpcodesXLATB(OpcodesXLATB *opcodes_xlatb)
{
  /* Code For OpcodesXLATB Goes Here */


}

void Skeleton::visitOpcodesXOR(OpcodesXOR *opcodes_xor)
{
  /* Code For OpcodesXOR Goes Here */


}


void Skeleton::visitListStatement(ListStatement *list_statement)
{
  for (ListStatement::iterator i = list_statement->begin() ; i != list_statement->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Skeleton::visitListMnemonicArgs(ListMnemonicArgs *list_mnemonic_args)
{
  for (ListMnemonicArgs::iterator i = list_mnemonic_args->begin() ; i != list_mnemonic_args->end() ; ++i)
  {
    (*i)->accept(this);
  }
}


void Skeleton::visitInteger(Integer x)
{
  /* Code for Integer Goes Here */
}

void Skeleton::visitChar(Char x)
{
  /* Code for Char Goes Here */
}

void Skeleton::visitDouble(Double x)
{
  /* Code for Double Goes Here */
}

void Skeleton::visitString(String x)
{
  /* Code for String Goes Here */
}

void Skeleton::visitIdent(Ident x)
{
  /* Code for Ident Goes Here */
}

void Skeleton::visitHex(Hex x)
{
  /* Code for Hex Goes Here */
}



