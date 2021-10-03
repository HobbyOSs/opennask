#ifndef LABEL_HPP
#define LABEL_HPP

#include <iostream>
#include "spdlog/spdlog.h"
#include "nask_defs.hpp"

struct LabelDstElement {
    std::string label; // ex) entry:
    long src_index;  // JMPのオペコードが始まる場所
    long dst_index;  // JMPの飛び先のラベルが始まる場所
    long rel_index;  // rel_offsetを格納する場所
    long rel_offset() {
        // offset = rel - dst
        log()->debug("dst_offs: {} - {} - 1",
                     std::to_string(src_index), std::to_string(rel_index));
        return src_index - rel_index - 1;
    };
};

struct LabelSrcElement {
    std::string label;   // ex) entry
    //OPERAND_KINDS operand;
    bool abs = false;
    long src_index;  // JMPのオペコードが始まる場所
    long dst_index;  // JMPの飛び先のラベルが始まる場所
    long rel_index;  // rel_offsetを格納する場所
    size_t offset_size; // オフセットの格納サイズを指定する
    long rel_offset() {
        // offset = rel - dst
        log()->debug("src_offs: {} - {} - 1",
                     std::to_string(dst_index), std::to_string(rel_index));
        return dst_index - rel_index - 1;
    };
};

// 処理の中でlabel情報の収集をする
typedef std::vector<LabelDstElement> LabelDstList;
typedef std::vector<LabelSrcElement> LabelSrcList;


#endif // ! LABEL_HPP
