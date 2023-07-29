#ifndef FRONT_END_EXT_HH
#define FRONT_END_EXT_HH


#include <asmjit/asmjit.h>
#include "para_token.hh"
#include "nask_defs.hpp"

// asmjitはデフォルトで32bitモードなので、辻褄あわせのため
class PrefixInfo {

public:
    bool require_67h = false; // Address size Prefix byte
    bool require_66h = false; // Operand size Prefix byte

    void set(OPENNASK_MODES, TParaToken&);
    void set(OPENNASK_MODES, TParaToken&, TParaToken&);
};


template <class F>
void FrontEnd::with_asmjit(F && f) {

    using namespace asmjit;
    PrefixInfo pp;
    CodeBuffer& buf = code_.textSection()->buffer();
    const size_t before_size = buf.size();

    f(*a_, pp);

    // asmjitはデフォルトは32bitモード
    // 0x67, 0x66の制御
    std::vector<uint8_t> old_data(buf.data(), buf.data() + buf.size());
    std::vector<uint8_t> new_data = {};
    new_data.reserve(old_data.size() + 2);

    auto it = old_data.begin();
    std::advance(it, before_size);
    std::copy(old_data.begin(), it, std::back_inserter(new_data));

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

    buf._size = new_data.size(); // コピー前にbufのサイズ情報を更新する
    memcpy(buf.begin(), new_data.data(), new_data.size());
}

#endif // ! FRONT_END_EXT_HH
