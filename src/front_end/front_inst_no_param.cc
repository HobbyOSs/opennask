#include <fstream>
#include <typeinfo>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "front_end.hh"

using namespace asmjit::x86;

//
// 引数なし命令は型引数にasmjitのx86::Inst::Idを渡して実装とする
//
template <Inst::Id id>
void FrontEnd::processEmit() {
    with_asmjit([&](Assembler& a, PrefixInfo& _) {
        a.emit(id);
    });
}

template void FrontEnd::processEmit<Inst::kIdAaa>();
template void FrontEnd::processEmit<Inst::kIdAas>();
template void FrontEnd::processEmit<Inst::kIdCbw>();
template void FrontEnd::processEmit<Inst::kIdCdq>();
template void FrontEnd::processEmit<Inst::kIdClc>();
template void FrontEnd::processEmit<Inst::kIdCld>();
template void FrontEnd::processEmit<Inst::kIdCli>();
template void FrontEnd::processEmit<Inst::kIdClts>();
template void FrontEnd::processEmit<Inst::kIdCmc>();
//template void FrontEnd::processEmit<Inst::kIdCmpsb>();
//template void FrontEnd::processEmit<Inst::kIdCmpsd>();
//template void FrontEnd::processEmit<Inst::kIdCmpsw>();
template void FrontEnd::processEmit<Inst::kIdCwd>();
template void FrontEnd::processEmit<Inst::kIdCwde>();
template void FrontEnd::processEmit<Inst::kIdDaa>();
template void FrontEnd::processEmit<Inst::kIdDas>();
template void FrontEnd::processEmit<Inst::kIdF2xm1>();
template void FrontEnd::processEmit<Inst::kIdFabs>();
template void FrontEnd::processEmit<Inst::kIdFchs>();
template void FrontEnd::processEmit<Inst::kIdFclex>();
template void FrontEnd::processEmit<Inst::kIdFcompp>();
template void FrontEnd::processEmit<Inst::kIdFcos>();
template void FrontEnd::processEmit<Inst::kIdFdecstp>();
//template void FrontEnd::processEmit<Inst::kIdFdisi>();
//template void FrontEnd::processEmit<Inst::kIdFeni>();
template void FrontEnd::processEmit<Inst::kIdFincstp>();
template void FrontEnd::processEmit<Inst::kIdFinit>();
template void FrontEnd::processEmit<Inst::kIdFld1>();
template void FrontEnd::processEmit<Inst::kIdFldl2e>();
template void FrontEnd::processEmit<Inst::kIdFldl2t>();
template void FrontEnd::processEmit<Inst::kIdFldlg2>();
template void FrontEnd::processEmit<Inst::kIdFldln2>();
template void FrontEnd::processEmit<Inst::kIdFldpi>();
template void FrontEnd::processEmit<Inst::kIdFldz>();
template void FrontEnd::processEmit<Inst::kIdFnclex>();
//template void FrontEnd::processEmit<Inst::kIdFndisi>();
//template void FrontEnd::processEmit<Inst::kIdFneni>();
template void FrontEnd::processEmit<Inst::kIdFninit>();
template void FrontEnd::processEmit<Inst::kIdFnop>();
template void FrontEnd::processEmit<Inst::kIdFpatan>();
template void FrontEnd::processEmit<Inst::kIdFptan>();
template void FrontEnd::processEmit<Inst::kIdFprem>();
template void FrontEnd::processEmit<Inst::kIdFprem1>();
template void FrontEnd::processEmit<Inst::kIdFrndint>();
template void FrontEnd::processEmit<Inst::kIdFscale>();
//template void FrontEnd::processEmit<Inst::kIdFsetpm>();
template void FrontEnd::processEmit<Inst::kIdFsin>();
template void FrontEnd::processEmit<Inst::kIdFsincos>();
template void FrontEnd::processEmit<Inst::kIdFsqrt>();
template void FrontEnd::processEmit<Inst::kIdFtst>();
template void FrontEnd::processEmit<Inst::kIdFucompp>();
template void FrontEnd::processEmit<Inst::kIdFxam>();
template void FrontEnd::processEmit<Inst::kIdFxtract>();
template void FrontEnd::processEmit<Inst::kIdFyl2x>();
template void FrontEnd::processEmit<Inst::kIdFyl2xp1>();
template void FrontEnd::processEmit<Inst::kIdHlt>();
//template void FrontEnd::processEmit<Inst::kIdInsb>();
//template void FrontEnd::processEmit<Inst::kIdInsd>();
//template void FrontEnd::processEmit<Inst::kIdInsw>();
template void FrontEnd::processEmit<Inst::kIdInt3>();
template void FrontEnd::processEmit<Inst::kIdInto>();
template void FrontEnd::processEmit<Inst::kIdInvd>();
template void FrontEnd::processEmit<Inst::kIdIret>();
template void FrontEnd::processEmit<Inst::kIdIretd>();
//template void FrontEnd::processEmit<Inst::kIdIretw>();
template void FrontEnd::processEmit<Inst::kIdLahf>();
template void FrontEnd::processEmit<Inst::kIdLeave>();
//template void FrontEnd::processEmit<Inst::kIdLodsb>();
//template void FrontEnd::processEmit<Inst::kIdLodsd>();
//template void FrontEnd::processEmit<Inst::kIdLodsw>();
//template void FrontEnd::processEmit<Inst::kIdMovsb>();
template void FrontEnd::processEmit<Inst::kIdMovsd>();
//template void FrontEnd::processEmit<Inst::kIdMovsw>();
template void FrontEnd::processEmit<Inst::kIdNop>();
//template void FrontEnd::processEmit<Inst::kIdOutsb>();
//template void FrontEnd::processEmit<Inst::kIdOutsd>();
//template void FrontEnd::processEmit<Inst::kIdOutsw>();
template void FrontEnd::processEmit<Inst::kIdPopa>();
template void FrontEnd::processEmit<Inst::kIdPopad>();
//template void FrontEnd::processEmit<Inst::kIdPopaw>();
template void FrontEnd::processEmit<Inst::kIdPopf>();
template void FrontEnd::processEmit<Inst::kIdPopfd>();
//template void FrontEnd::processEmit<Inst::kIdPopfw>();
template void FrontEnd::processEmit<Inst::kIdPusha>();
template void FrontEnd::processEmit<Inst::kIdPushad>();
//template void FrontEnd::processEmit<Inst::kIdPushaw>();
template void FrontEnd::processEmit<Inst::kIdPushf>();
template void FrontEnd::processEmit<Inst::kIdPushfd>();
//template void FrontEnd::processEmit<Inst::kIdPushfw>();
template void FrontEnd::processEmit<Inst::kIdSahf>();
//template void FrontEnd::processEmit<Inst::kIdScasb>();
//template void FrontEnd::processEmit<Inst::kIdScasd>();
//template void FrontEnd::processEmit<Inst::kIdScasw>();
template void FrontEnd::processEmit<Inst::kIdStc>();
template void FrontEnd::processEmit<Inst::kIdStd>();
template void FrontEnd::processEmit<Inst::kIdSti>();
//template void FrontEnd::processEmit<Inst::kIdStosb>();
//template void FrontEnd::processEmit<Inst::kIdStosd>();
//template void FrontEnd::processEmit<Inst::kIdStosw>();
//template void FrontEnd::processEmit<Inst::kIdWait>();
template void FrontEnd::processEmit<Inst::kIdWbinvd>();
template void FrontEnd::processEmit<Inst::kIdXlatb>();
