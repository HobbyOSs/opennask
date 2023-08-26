#ifndef FRONT_END_EXT_HH
#define FRONT_END_EXT_HH


#include <asmjit/asmjit.h>
#include "para_token.hh"
#include "nask_parse_common.hh"


// asmjitはデフォルトで32bitモードなので、辻褄あわせのため
class PrefixInfo {

public:
    bool require_67h = false; // Address size Prefix byte
    bool require_66h = false; // Operand size Prefix byte

    void set(OPENNASK_MODES, TParaToken& dst);
    void set(OPENNASK_MODES, TParaToken& dst, TParaToken& src);
};


template <class F>
void FrontEnd::with_asmjit(F && f) {

    using namespace asmjit;
    PrefixInfo pp;
    CodeBuffer& buf = code_.textSection()->buffer();
    const size_t before_size = buf.size();

    f(*a_, pp);

    const size_t after_size = buf.size();
    if (before_size == after_size) {
        return; // 何も出力がないときはskip
    }

    // asmjitはデフォルトは32bitモード
    // 0x67, 0x66の制御
    std::vector<uint8_t> old_data(buf.data(), buf.data() + buf.size());
    std::vector<uint8_t> new_data = {};

    auto it = old_data.begin();
    std::advance(it, before_size);
    std::copy(old_data.begin(), it, std::back_inserter(new_data));

    log()->debug("[pass2] require 67H={}, 66H={}", pp.require_67h, pp.require_66h);
    if (pp.require_67h) {
        if (*it == 0x67) {
            std::advance(it, 1);
        } else {
            new_data.push_back(0x67);
        }
    } else {
        if (*it == 0x67)
            std::advance(it, 1);
    }

    if (pp.require_66h) {
        if (*it == 0x66) {
            std::advance(it, 1);
        } else {
            new_data.push_back(0x66);
        }
    } else {
        if (*it == 0x66)
            std::advance(it, 1);
    }
    std::copy(it, old_data.end(), std::back_inserter(new_data));

    // asmjitのbufferに関する調整
    //   memset: 念の為操作したポインタ周辺をゼロで埋める
    //   memcpy: 修正したバイナリをasmjitのCodeBufferに反映する
    //   buf._size: bufのサイズ情報を更新する(自動で更新されない)
    //   _bufferPtr: 次にバイナリを書き込む際の位置をここで管理しているので増減した分をずらす
    memset(buf._data, 0, buf.size());
    memcpy(buf.begin(), new_data.data(), new_data.size());
    buf._size = new_data.size();
    a_->_bufferPtr = buf.data() + new_data.size(); // asmjit/src/asmjit/core/assembler.h L.40
}

#endif // ! FRONT_END_EXT_HH
