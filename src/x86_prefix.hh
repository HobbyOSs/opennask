#ifndef X86_PREFIX_HH
#define X86_PREFIX_HH


#include "para_token.hh"
#include "nask_parse_common.hh"


// asmjitはデフォルトで32bitモードなので、辻褄あわせのため
class PrefixInfo {

public:
    bool require_67h = false; // Address size Prefix byte
    bool require_66h = false; // Operand size Prefix byte

    void set(OPENNASK_MODES, const TParaToken& dst);
    void set(OPENNASK_MODES, const TParaToken& dst, const TParaToken& src);
};

#endif // ! X86_PREFIX_HH
