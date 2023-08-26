#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "pass1_strategy.hh"
#include "x86.hh"

using namespace asmjit::x86;

//
// 引数なし命令は型引数にasmjitのx86::Inst::Idと機械語サイズを渡して実装とする
//
template <Inst::Id id, int N>
void Pass1Strategy::processCalc() {
    loc += N;
    log()->debug("[pass1] LOC = {}({:x})", loc, loc);
    return;
}

template void Pass1Strategy::processCalc<Inst::kIdAaa, 1>();
template void Pass1Strategy::processCalc<Inst::kIdAas, 1>();
template void Pass1Strategy::processCalc<Inst::kIdCbw, 1>();
template void Pass1Strategy::processCalc<Inst::kIdCdq, 1>();
template void Pass1Strategy::processCalc<Inst::kIdClc, 1>();
template void Pass1Strategy::processCalc<Inst::kIdCld, 1>();
template void Pass1Strategy::processCalc<Inst::kIdCli, 1>();
template void Pass1Strategy::processCalc<Inst::kIdClts, 2>();
template void Pass1Strategy::processCalc<Inst::kIdCmc, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdCmpsb, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdCmpsd, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdCmpsw, 1>();
template void Pass1Strategy::processCalc<Inst::kIdCwd, 1>();
template void Pass1Strategy::processCalc<Inst::kIdCwde, 1>();
template void Pass1Strategy::processCalc<Inst::kIdDaa, 1>();
template void Pass1Strategy::processCalc<Inst::kIdDas, 1>();
template void Pass1Strategy::processCalc<Inst::kIdF2xm1, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFabs, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFchs, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFclex, 3>();
template void Pass1Strategy::processCalc<Inst::kIdFcompp, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFcos, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFdecstp, 2>();
//template void Pass1Strategy::processCalc<Inst::kIdFdisi, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdFeni, 1>();
template void Pass1Strategy::processCalc<Inst::kIdFincstp, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFinit, 3>();
template void Pass1Strategy::processCalc<Inst::kIdFld1, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFldl2e, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFldl2t, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFldlg2, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFldln2, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFldpi, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFldz, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFnclex, 2>();
//template void Pass1Strategy::processCalc<Inst::kIdFndisi, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdFneni, 1>();
template void Pass1Strategy::processCalc<Inst::kIdFninit, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFnop, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFpatan, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFptan, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFprem, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFprem1, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFrndint, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFscale, 2>();
//template void Pass1Strategy::processCalc<Inst::kIdFsetpm, 1>();
template void Pass1Strategy::processCalc<Inst::kIdFsin, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFsincos, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFsqrt, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFtst, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFucompp, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFxam, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFxtract, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFyl2x, 2>();
template void Pass1Strategy::processCalc<Inst::kIdFyl2xp1, 2>();
template void Pass1Strategy::processCalc<Inst::kIdHlt, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdInsb, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdInsd, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdInsw, 1>();
template void Pass1Strategy::processCalc<Inst::kIdInt3, 1>();
template void Pass1Strategy::processCalc<Inst::kIdInto, 1>();
template void Pass1Strategy::processCalc<Inst::kIdInvd, 2>();
template void Pass1Strategy::processCalc<Inst::kIdIret, 1>();
template void Pass1Strategy::processCalc<Inst::kIdIretd, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdIretw, 1>();
template void Pass1Strategy::processCalc<Inst::kIdLahf, 1>();
template void Pass1Strategy::processCalc<Inst::kIdLeave, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdLodsb, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdLodsd, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdLodsw, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdMovsb, 1>();
template void Pass1Strategy::processCalc<Inst::kIdMovsd, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdMovsw, 1>();
template void Pass1Strategy::processCalc<Inst::kIdNop, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdOutsb, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdOutsd, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdOutsw, 1>();
template void Pass1Strategy::processCalc<Inst::kIdPopa, 1>();
template void Pass1Strategy::processCalc<Inst::kIdPopad, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdPopaw, 1>();
template void Pass1Strategy::processCalc<Inst::kIdPopf, 1>();
template void Pass1Strategy::processCalc<Inst::kIdPopfd, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdPopfw, 1>();
template void Pass1Strategy::processCalc<Inst::kIdPusha, 1>();
template void Pass1Strategy::processCalc<Inst::kIdPushad, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdPushaw, 1>();
template void Pass1Strategy::processCalc<Inst::kIdPushf, 1>();
template void Pass1Strategy::processCalc<Inst::kIdPushfd, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdPushfw, 1>();
template void Pass1Strategy::processCalc<Inst::kIdSahf, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdScasb, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdScasd, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdScasw, 1>();
template void Pass1Strategy::processCalc<Inst::kIdStc, 1>();
template void Pass1Strategy::processCalc<Inst::kIdStd, 1>();
template void Pass1Strategy::processCalc<Inst::kIdSti, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdStosb, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdStosd, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdStosw, 1>();
//template void Pass1Strategy::processCalc<Inst::kIdWait, 1>();
template void Pass1Strategy::processCalc<Inst::kIdWbinvd, 2>();
template void Pass1Strategy::processCalc<Inst::kIdXlatb, 1>();
