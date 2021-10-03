#ifndef LABEL_HPP
#define LABEL_HPP

#include <iostream>
#include "spdlog/spdlog.h"
#include "nask_defs.hpp"

// 名付け的にsource, destinationが逆な気もしたが
// gotoでジャンプするときをイメージした際に;
// ":"がついたラベルがジャンプ先(dst)
// ":"がついてないラベルがジャンプ元(src)
// と考えてこの命名

struct LabelDstElement {
    std::string label; // ex) entry:
    long src_index;    // JMPのオペコードが始まる場所
    long dst_index;    // JMPの飛び先のラベルが始まる場所
    long rel_index;    // rel_offsetを格納する場所
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
    bool abs = false;    // オフセットの計算時に相対ではなく絶対位置の場合true
    long src_index;      // JMPのオペコードが始まる場所
    long dst_index;      // JMPの飛び先のラベルが始まる場所
    long rel_index;      // rel_offsetを格納する場所
    size_t offset_size;  // オフセットの格納サイズを指定する
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


namespace LabelJmp {
    // ":"がついたラベルがジャンプ先(dst)を扱う関数
    void store_label_dst(std::string, LabelDstList&, std::vector<uint8_t>&);
    void update_label_dst_offset(std::string, LabelSrcList&, uint32_t, std::vector<uint8_t>&);
    bool dst_is_stored(std::string label_dst);

    // ":"がついてないラベルがジャンプ元(src)を扱う関数
    void store_label_src(std::string, LabelSrcList& ,std::vector<uint8_t>&, bool abs = false, size_t offset_size = imm8);
    void update_label_src_offset(std::string, LabelDstList&, uint8_t, std::vector<uint8_t>&);

    void set_word_into_binout(const uint16_t&, std::vector<uint8_t>&, size_t start_index = 0);
    void set_dword_into_binout(const uint32_t&, std::vector<uint8_t>&, size_t start_index = 0);
};

#endif // ! LABEL_HPP
