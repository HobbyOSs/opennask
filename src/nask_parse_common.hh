#ifndef NASK_PARSE_COMMON_HH
#define NASK_PARSE_COMMON_HH

#include <map>
#include <functional>
#include "spdlog/spdlog.h"
#include "para_token.hh"


static const std::shared_ptr<spdlog::logger> log() {
    return spdlog::get("opennask");
}

// 現在のnaskのモード
// [BITS 16], [BITS 32], [BITS 64]で指定する
// デフォルトは16bitモード
enum OPENNASK_MODES {
    ID_16BIT_MODE = 0,
    ID_32BIT_MODE,
    ID_64BIT_MODE
};

static constexpr const char* OPENNASK_MODES_NAMES[] = {
    "16BIT_MODE",
    "32BIT_MODE",
    "64BIT_MODE"
};

using NimCallback = std::function<void(std::vector<TParaToken>&)>;
using FuncsType = std::map<std::string, NimCallback>;

#endif // ! NASK_PARSE_COMMON_HH
