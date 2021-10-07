#ifndef SKELETON_HEADER
#define SKELETON_HEADER
/* You might want to change the above name. */

#include "absyn.hh"


class Skeleton : public Visitor
{
public:
  void visitProgram(Program *p);
  void visitStatement(Statement *p);
  void visitMnemonicArgs(MnemonicArgs *p);
  void visitExp(Exp *p);
  void visitFactor(Factor *p);
  void visitConfigType(ConfigType *p);
  void visitDataType(DataType *p);
  void visitOpcode(Opcode *p);
  void visitProg(Prog *p);
  void visitLabelStmt(LabelStmt *p);
  void visitDeclareStmt(DeclareStmt *p);
  void visitConfigStmt(ConfigStmt *p);
  void visitMnemonicStmt(MnemonicStmt *p);
  void visitOpcodeStmt(OpcodeStmt *p);
  void visitMnemoArg(MnemoArg *p);
  void visitPlusExp(PlusExp *p);
  void visitMinusExp(MinusExp *p);
  void visitMulExp(MulExp *p);
  void visitDivExp(DivExp *p);
  void visitModExp(ModExp *p);
  void visitIndirectAddrExp(IndirectAddrExp *p);
  void visitDatatypeExp(DatatypeExp *p);
  void visitRangeExp(RangeExp *p);
  void visitImmExp(ImmExp *p);
  void visitNumberFactor(NumberFactor *p);
  void visitHexFactor(HexFactor *p);
  void visitIdentFactor(IdentFactor *p);
  void visitStringFactor(StringFactor *p);
  void visitBitsConfig(BitsConfig *p);
  void visitInstConfig(InstConfig *p);
  void visitOptiConfig(OptiConfig *p);
  void visitFormConfig(FormConfig *p);
  void visitPaddConfig(PaddConfig *p);
  void visitPadsConfig(PadsConfig *p);
  void visitSectConfig(SectConfig *p);
  void visitAbsoConfig(AbsoConfig *p);
  void visitFileConfig(FileConfig *p);
  void visitByteDataType(ByteDataType *p);
  void visitWordDataType(WordDataType *p);
  void visitDwordDataType(DwordDataType *p);
  void visitOpcodesAAA(OpcodesAAA *p);
  void visitOpcodesAAD(OpcodesAAD *p);
  void visitOpcodesAAS(OpcodesAAS *p);
  void visitOpcodesAAM(OpcodesAAM *p);
  void visitOpcodesADC(OpcodesADC *p);
  void visitOpcodesADD(OpcodesADD *p);
  void visitOpcodesAND(OpcodesAND *p);
  void visitOpcodesALIGN(OpcodesALIGN *p);
  void visitOpcodesALIGNB(OpcodesALIGNB *p);
  void visitOpcodesARPL(OpcodesARPL *p);
  void visitOpcodesBOUND(OpcodesBOUND *p);
  void visitOpcodesBSF(OpcodesBSF *p);
  void visitOpcodesBSR(OpcodesBSR *p);
  void visitOpcodesBSWAP(OpcodesBSWAP *p);
  void visitOpcodesBT(OpcodesBT *p);
  void visitOpcodesBTC(OpcodesBTC *p);
  void visitOpcodesBTR(OpcodesBTR *p);
  void visitOpcodesBTS(OpcodesBTS *p);
  void visitOpcodesCALL(OpcodesCALL *p);
  void visitOpcodesCBW(OpcodesCBW *p);
  void visitOpcodesCDQ(OpcodesCDQ *p);
  void visitOpcodesCLC(OpcodesCLC *p);
  void visitOpcodesCLD(OpcodesCLD *p);
  void visitOpcodesCLI(OpcodesCLI *p);
  void visitOpcodesCLTS(OpcodesCLTS *p);
  void visitOpcodesCMC(OpcodesCMC *p);
  void visitOpcodesCMP(OpcodesCMP *p);
  void visitOpcodesCMPSB(OpcodesCMPSB *p);
  void visitOpcodesCMPSD(OpcodesCMPSD *p);
  void visitOpcodesCMPSW(OpcodesCMPSW *p);
  void visitOpcodesCMPXCHG(OpcodesCMPXCHG *p);
  void visitOpcodesCPUID(OpcodesCPUID *p);
  void visitOpcodesCWD(OpcodesCWD *p);
  void visitOpcodesCWDE(OpcodesCWDE *p);
  void visitOpcodesDAA(OpcodesDAA *p);
  void visitOpcodesDAS(OpcodesDAS *p);
  void visitOpcodesDB(OpcodesDB *p);
  void visitOpcodesDD(OpcodesDD *p);
  void visitOpcodesDEC(OpcodesDEC *p);
  void visitOpcodesDIV(OpcodesDIV *p);
  void visitOpcodesDQ(OpcodesDQ *p);
  void visitOpcodesDT(OpcodesDT *p);
  void visitOpcodesDW(OpcodesDW *p);
  void visitOpcodesEND(OpcodesEND *p);
  void visitOpcodesENTER(OpcodesENTER *p);
  void visitOpcodesEXTERN(OpcodesEXTERN *p);
  void visitOpcodesF2XM1(OpcodesF2XM1 *p);
  void visitOpcodesFABS(OpcodesFABS *p);
  void visitOpcodesFADD(OpcodesFADD *p);
  void visitOpcodesFADDP(OpcodesFADDP *p);
  void visitOpcodesFBLD(OpcodesFBLD *p);
  void visitOpcodesFBSTP(OpcodesFBSTP *p);
  void visitOpcodesFCHS(OpcodesFCHS *p);
  void visitOpcodesFCLEX(OpcodesFCLEX *p);
  void visitOpcodesFCOM(OpcodesFCOM *p);
  void visitOpcodesFCOMP(OpcodesFCOMP *p);
  void visitOpcodesFCOMPP(OpcodesFCOMPP *p);
  void visitOpcodesFCOS(OpcodesFCOS *p);
  void visitOpcodesFDECSTP(OpcodesFDECSTP *p);
  void visitOpcodesFDISI(OpcodesFDISI *p);
  void visitOpcodesFDIV(OpcodesFDIV *p);
  void visitOpcodesFDIVP(OpcodesFDIVP *p);
  void visitOpcodesFDIVR(OpcodesFDIVR *p);
  void visitOpcodesFDIVRP(OpcodesFDIVRP *p);
  void visitOpcodesFENI(OpcodesFENI *p);
  void visitOpcodesFFREE(OpcodesFFREE *p);
  void visitOpcodesFIADD(OpcodesFIADD *p);
  void visitOpcodesFICOM(OpcodesFICOM *p);
  void visitOpcodesFICOMP(OpcodesFICOMP *p);
  void visitOpcodesFIDIV(OpcodesFIDIV *p);
  void visitOpcodesFIDIVR(OpcodesFIDIVR *p);
  void visitOpcodesFILD(OpcodesFILD *p);
  void visitOpcodesFIMUL(OpcodesFIMUL *p);
  void visitOpcodesFINCSTP(OpcodesFINCSTP *p);
  void visitOpcodesFINIT(OpcodesFINIT *p);
  void visitOpcodesFIST(OpcodesFIST *p);
  void visitOpcodesFISTP(OpcodesFISTP *p);
  void visitOpcodesFISUB(OpcodesFISUB *p);
  void visitOpcodesFISUBR(OpcodesFISUBR *p);
  void visitOpcodesFLD(OpcodesFLD *p);
  void visitOpcodesFLD1(OpcodesFLD1 *p);
  void visitOpcodesFLDCW(OpcodesFLDCW *p);
  void visitOpcodesFLDENV(OpcodesFLDENV *p);
  void visitOpcodesFLDL2E(OpcodesFLDL2E *p);
  void visitOpcodesFLDL2T(OpcodesFLDL2T *p);
  void visitOpcodesFLDLG2(OpcodesFLDLG2 *p);
  void visitOpcodesFLDLN2(OpcodesFLDLN2 *p);
  void visitOpcodesFLDPI(OpcodesFLDPI *p);
  void visitOpcodesFLDZ(OpcodesFLDZ *p);
  void visitOpcodesFMUL(OpcodesFMUL *p);
  void visitOpcodesFMULP(OpcodesFMULP *p);
  void visitOpcodesFNCLEX(OpcodesFNCLEX *p);
  void visitOpcodesFNDISI(OpcodesFNDISI *p);
  void visitOpcodesFNENI(OpcodesFNENI *p);
  void visitOpcodesFNINIT(OpcodesFNINIT *p);
  void visitOpcodesFNOP(OpcodesFNOP *p);
  void visitOpcodesFNSAVE(OpcodesFNSAVE *p);
  void visitOpcodesFNSTCW(OpcodesFNSTCW *p);
  void visitOpcodesFNSTENV(OpcodesFNSTENV *p);
  void visitOpcodesFNSTSW(OpcodesFNSTSW *p);
  void visitOpcodesFPATAN(OpcodesFPATAN *p);
  void visitOpcodesFPTAN(OpcodesFPTAN *p);
  void visitOpcodesFPREM(OpcodesFPREM *p);
  void visitOpcodesFPREM1(OpcodesFPREM1 *p);
  void visitOpcodesFRNDINT(OpcodesFRNDINT *p);
  void visitOpcodesFRSTOR(OpcodesFRSTOR *p);
  void visitOpcodesFSAVE(OpcodesFSAVE *p);
  void visitOpcodesFSCALE(OpcodesFSCALE *p);
  void visitOpcodesFSETPM(OpcodesFSETPM *p);
  void visitOpcodesFSIN(OpcodesFSIN *p);
  void visitOpcodesFSINCOS(OpcodesFSINCOS *p);
  void visitOpcodesFSQRT(OpcodesFSQRT *p);
  void visitOpcodesFST(OpcodesFST *p);
  void visitOpcodesFSTCW(OpcodesFSTCW *p);
  void visitOpcodesFSTENV(OpcodesFSTENV *p);
  void visitOpcodesFSTP(OpcodesFSTP *p);
  void visitOpcodesFSTSW(OpcodesFSTSW *p);
  void visitOpcodesFSUB(OpcodesFSUB *p);
  void visitOpcodesFSUBP(OpcodesFSUBP *p);
  void visitOpcodesFSUBR(OpcodesFSUBR *p);
  void visitOpcodesFSUBRP(OpcodesFSUBRP *p);
  void visitOpcodesFTST(OpcodesFTST *p);
  void visitOpcodesFUCOM(OpcodesFUCOM *p);
  void visitOpcodesFUCOMP(OpcodesFUCOMP *p);
  void visitOpcodesFUCOMPP(OpcodesFUCOMPP *p);
  void visitOpcodesFXAM(OpcodesFXAM *p);
  void visitOpcodesFXCH(OpcodesFXCH *p);
  void visitOpcodesFXTRACT(OpcodesFXTRACT *p);
  void visitOpcodesFYL2X(OpcodesFYL2X *p);
  void visitOpcodesFYL2XP1(OpcodesFYL2XP1 *p);
  void visitOpcodesHLT(OpcodesHLT *p);
  void visitOpcodesIDIV(OpcodesIDIV *p);
  void visitOpcodesIMUL(OpcodesIMUL *p);
  void visitOpcodesIN(OpcodesIN *p);
  void visitOpcodesINC(OpcodesINC *p);
  void visitOpcodesINCO(OpcodesINCO *p);
  void visitOpcodesINSB(OpcodesINSB *p);
  void visitOpcodesINSD(OpcodesINSD *p);
  void visitOpcodesINSW(OpcodesINSW *p);
  void visitOpcodesINT(OpcodesINT *p);
  void visitOpcodesINT3(OpcodesINT3 *p);
  void visitOpcodesINTO(OpcodesINTO *p);
  void visitOpcodesINVD(OpcodesINVD *p);
  void visitOpcodesINVLPG(OpcodesINVLPG *p);
  void visitOpcodesIRET(OpcodesIRET *p);
  void visitOpcodesIRETD(OpcodesIRETD *p);
  void visitOpcodesIRETW(OpcodesIRETW *p);
  void visitOpcodesJA(OpcodesJA *p);
  void visitOpcodesJAE(OpcodesJAE *p);
  void visitOpcodesJB(OpcodesJB *p);
  void visitOpcodesJBE(OpcodesJBE *p);
  void visitOpcodesJC(OpcodesJC *p);
  void visitOpcodesJCXZ(OpcodesJCXZ *p);
  void visitOpcodesJE(OpcodesJE *p);
  void visitOpcodesJECXZ(OpcodesJECXZ *p);
  void visitOpcodesJG(OpcodesJG *p);
  void visitOpcodesJGE(OpcodesJGE *p);
  void visitOpcodesJL(OpcodesJL *p);
  void visitOpcodesJLE(OpcodesJLE *p);
  void visitOpcodesJMP(OpcodesJMP *p);
  void visitOpcodesJNA(OpcodesJNA *p);
  void visitOpcodesJNAE(OpcodesJNAE *p);
  void visitOpcodesJNB(OpcodesJNB *p);
  void visitOpcodesJNBE(OpcodesJNBE *p);
  void visitOpcodesJNC(OpcodesJNC *p);
  void visitOpcodesJNE(OpcodesJNE *p);
  void visitOpcodesJNG(OpcodesJNG *p);
  void visitOpcodesJNGE(OpcodesJNGE *p);
  void visitOpcodesJNL(OpcodesJNL *p);
  void visitOpcodesJNLE(OpcodesJNLE *p);
  void visitOpcodesJNO(OpcodesJNO *p);
  void visitOpcodesJNP(OpcodesJNP *p);
  void visitOpcodesJNS(OpcodesJNS *p);
  void visitOpcodesJNZ(OpcodesJNZ *p);
  void visitOpcodesJO(OpcodesJO *p);
  void visitOpcodesJP(OpcodesJP *p);
  void visitOpcodesJPE(OpcodesJPE *p);
  void visitOpcodesJPO(OpcodesJPO *p);
  void visitOpcodesJS(OpcodesJS *p);
  void visitOpcodesJZ(OpcodesJZ *p);
  void visitOpcodesLAHF(OpcodesLAHF *p);
  void visitOpcodesLAR(OpcodesLAR *p);
  void visitOpcodesLDS(OpcodesLDS *p);
  void visitOpcodesLEA(OpcodesLEA *p);
  void visitOpcodesLEAVE(OpcodesLEAVE *p);
  void visitOpcodesLES(OpcodesLES *p);
  void visitOpcodesLFS(OpcodesLFS *p);
  void visitOpcodesLGDT(OpcodesLGDT *p);
  void visitOpcodesLGS(OpcodesLGS *p);
  void visitOpcodesLIDT(OpcodesLIDT *p);
  void visitOpcodesLLDT(OpcodesLLDT *p);
  void visitOpcodesLMSW(OpcodesLMSW *p);
  void visitOpcodesLOCK(OpcodesLOCK *p);
  void visitOpcodesLODSB(OpcodesLODSB *p);
  void visitOpcodesLODSD(OpcodesLODSD *p);
  void visitOpcodesLODSW(OpcodesLODSW *p);
  void visitOpcodesLOOP(OpcodesLOOP *p);
  void visitOpcodesLOOPE(OpcodesLOOPE *p);
  void visitOpcodesLOOPNE(OpcodesLOOPNE *p);
  void visitOpcodesLOOPNZ(OpcodesLOOPNZ *p);
  void visitOpcodesLOOPZ(OpcodesLOOPZ *p);
  void visitOpcodesLSL(OpcodesLSL *p);
  void visitOpcodesLSS(OpcodesLSS *p);
  void visitOpcodesLTR(OpcodesLTR *p);
  void visitOpcodesMOV(OpcodesMOV *p);
  void visitOpcodesMOVSB(OpcodesMOVSB *p);
  void visitOpcodesMOVSD(OpcodesMOVSD *p);
  void visitOpcodesMOVSW(OpcodesMOVSW *p);
  void visitOpcodesMOVSX(OpcodesMOVSX *p);
  void visitOpcodesMOVZX(OpcodesMOVZX *p);
  void visitOpcodesMUL(OpcodesMUL *p);
  void visitOpcodesNEG(OpcodesNEG *p);
  void visitOpcodesNOP(OpcodesNOP *p);
  void visitOpcodesNOT(OpcodesNOT *p);
  void visitOpcodesOR(OpcodesOR *p);
  void visitOpcodesORG(OpcodesORG *p);
  void visitOpcodesOUT(OpcodesOUT *p);
  void visitOpcodesOUTSB(OpcodesOUTSB *p);
  void visitOpcodesOUTSD(OpcodesOUTSD *p);
  void visitOpcodesOUTSW(OpcodesOUTSW *p);
  void visitOpcodesPOP(OpcodesPOP *p);
  void visitOpcodesPOPA(OpcodesPOPA *p);
  void visitOpcodesPOPAD(OpcodesPOPAD *p);
  void visitOpcodesPOPAW(OpcodesPOPAW *p);
  void visitOpcodesPOPF(OpcodesPOPF *p);
  void visitOpcodesPOPFD(OpcodesPOPFD *p);
  void visitOpcodesPOPFW(OpcodesPOPFW *p);
  void visitOpcodesPUSH(OpcodesPUSH *p);
  void visitOpcodesPUSHA(OpcodesPUSHA *p);
  void visitOpcodesPUSHD(OpcodesPUSHD *p);
  void visitOpcodesPUSHAD(OpcodesPUSHAD *p);
  void visitOpcodesPUSHAW(OpcodesPUSHAW *p);
  void visitOpcodesPUSHF(OpcodesPUSHF *p);
  void visitOpcodesPUSHFD(OpcodesPUSHFD *p);
  void visitOpcodesPUSHFW(OpcodesPUSHFW *p);
  void visitOpcodesRCL(OpcodesRCL *p);
  void visitOpcodesRCR(OpcodesRCR *p);
  void visitOpcodesRDMSR(OpcodesRDMSR *p);
  void visitOpcodesRDPMC(OpcodesRDPMC *p);
  void visitOpcodesREP(OpcodesREP *p);
  void visitOpcodesREPE(OpcodesREPE *p);
  void visitOpcodesREPNE(OpcodesREPNE *p);
  void visitOpcodesREPNZ(OpcodesREPNZ *p);
  void visitOpcodesREPZ(OpcodesREPZ *p);
  void visitOpcodesRESB(OpcodesRESB *p);
  void visitOpcodesRESD(OpcodesRESD *p);
  void visitOpcodesRESQ(OpcodesRESQ *p);
  void visitOpcodesREST(OpcodesREST *p);
  void visitOpcodesRESW(OpcodesRESW *p);
  void visitOpcodesRET(OpcodesRET *p);
  void visitOpcodesRETF(OpcodesRETF *p);
  void visitOpcodesRETN(OpcodesRETN *p);
  void visitOpcodesROL(OpcodesROL *p);
  void visitOpcodesROR(OpcodesROR *p);
  void visitOpcodesRSM(OpcodesRSM *p);
  void visitOpcodesSAHF(OpcodesSAHF *p);
  void visitOpcodesSAL(OpcodesSAL *p);
  void visitOpcodesSAR(OpcodesSAR *p);
  void visitOpcodesSBB(OpcodesSBB *p);
  void visitOpcodesSCASB(OpcodesSCASB *p);
  void visitOpcodesSCASD(OpcodesSCASD *p);
  void visitOpcodesSCASW(OpcodesSCASW *p);
  void visitOpcodesSETA(OpcodesSETA *p);
  void visitOpcodesSETAE(OpcodesSETAE *p);
  void visitOpcodesSETB(OpcodesSETB *p);
  void visitOpcodesSETBE(OpcodesSETBE *p);
  void visitOpcodesSETC(OpcodesSETC *p);
  void visitOpcodesSETE(OpcodesSETE *p);
  void visitOpcodesSETG(OpcodesSETG *p);
  void visitOpcodesSETGE(OpcodesSETGE *p);
  void visitOpcodesSETL(OpcodesSETL *p);
  void visitOpcodesSETLE(OpcodesSETLE *p);
  void visitOpcodesSETNA(OpcodesSETNA *p);
  void visitOpcodesSETNAE(OpcodesSETNAE *p);
  void visitOpcodesSETNB(OpcodesSETNB *p);
  void visitOpcodesSETNBE(OpcodesSETNBE *p);
  void visitOpcodesSETNC(OpcodesSETNC *p);
  void visitOpcodesSETNE(OpcodesSETNE *p);
  void visitOpcodesSETNG(OpcodesSETNG *p);
  void visitOpcodesSETNGE(OpcodesSETNGE *p);
  void visitOpcodesSETNL(OpcodesSETNL *p);
  void visitOpcodesSETNLE(OpcodesSETNLE *p);
  void visitOpcodesSETNO(OpcodesSETNO *p);
  void visitOpcodesSETNP(OpcodesSETNP *p);
  void visitOpcodesSETNS(OpcodesSETNS *p);
  void visitOpcodesSETNZ(OpcodesSETNZ *p);
  void visitOpcodesSETO(OpcodesSETO *p);
  void visitOpcodesSETP(OpcodesSETP *p);
  void visitOpcodesSETPE(OpcodesSETPE *p);
  void visitOpcodesSETPO(OpcodesSETPO *p);
  void visitOpcodesSETS(OpcodesSETS *p);
  void visitOpcodesSETZ(OpcodesSETZ *p);
  void visitOpcodesSGDT(OpcodesSGDT *p);
  void visitOpcodesSHL(OpcodesSHL *p);
  void visitOpcodesSHLD(OpcodesSHLD *p);
  void visitOpcodesSHR(OpcodesSHR *p);
  void visitOpcodesSHRD(OpcodesSHRD *p);
  void visitOpcodesSIDT(OpcodesSIDT *p);
  void visitOpcodesSLDT(OpcodesSLDT *p);
  void visitOpcodesSMSW(OpcodesSMSW *p);
  void visitOpcodesSTC(OpcodesSTC *p);
  void visitOpcodesSTD(OpcodesSTD *p);
  void visitOpcodesSTI(OpcodesSTI *p);
  void visitOpcodesSTOSB(OpcodesSTOSB *p);
  void visitOpcodesSTOSD(OpcodesSTOSD *p);
  void visitOpcodesSTOSW(OpcodesSTOSW *p);
  void visitOpcodesSTR(OpcodesSTR *p);
  void visitOpcodesSUB(OpcodesSUB *p);
  void visitOpcodesTEST(OpcodesTEST *p);
  void visitOpcodesTIMES(OpcodesTIMES *p);
  void visitOpcodesUD2(OpcodesUD2 *p);
  void visitOpcodesVERR(OpcodesVERR *p);
  void visitOpcodesVERW(OpcodesVERW *p);
  void visitOpcodesWAIT(OpcodesWAIT *p);
  void visitOpcodesWBINVD(OpcodesWBINVD *p);
  void visitOpcodesWRMSR(OpcodesWRMSR *p);
  void visitOpcodesXADD(OpcodesXADD *p);
  void visitOpcodesXCHG(OpcodesXCHG *p);
  void visitOpcodesXLATB(OpcodesXLATB *p);
  void visitOpcodesXOR(OpcodesXOR *p);
  void visitListStatement(ListStatement *p);
  void visitListMnemonicArgs(ListMnemonicArgs *p);

  void visitInteger(Integer x);
  void visitChar(Char x);
  void visitDouble(Double x);
  void visitString(String x);
  void visitIdent(Ident x);
  void visitHex(Hex x);
  void visitLabel(Label x);
  void visitNL(NL x);

};


#endif
