#include "label.hpp"
#include "matchit.h"

namespace LabelJmp {
    using namespace matchit; // パターンマッチ

    // label: (label_dstと呼ぶ)
    // 1) label_dstの位置を記録する → label_dst_list
    void store_label_dst(std::string label_dst, LabelDstList& label_dst_list, std::vector<uint8_t>& binout_container) {
        log()->debug("stored label: {} bin[{}]", label_dst, binout_container.size());
        LabelDstElement elem;
        elem.label = label_dst;
        elem.src_index = binout_container.size();
        label_dst_list.push_back(elem);
    }

    // label: (label_dstと呼ぶ)
    // 2) 同名のlabel_srcが保存されていれば、オフセット値を計算して終了
    void update_label_dst_offset(
        std::string label_dst,
        LabelSrcList& label_src_list,
        uint32_t dollar_position, // $ の位置
        std::vector<uint8_t>& binout_container
    ) {

        for (auto it = std::begin(label_src_list); it != std::end(label_src_list); ++it) {
            LabelSrcElement elem(*it);
            if (elem.label != label_dst)
                continue;

            // 処理の開始
            elem.dst_index = binout_container.size();

            match(std::make_tuple(elem.abs, elem.offset_size))(
                // ORGからの絶対値でオフセットを設定する
                pattern | ds(true, imm32) = [&] {
                    log()->debug("imm32: update_label_dst_offset bin[{}, {}, {}, {}] = 0x{:02x}",
                                 elem.rel_index,
                                 elem.rel_index + 1,
                                 elem.rel_index + 2,
                                 elem.rel_index + 3,
                                 dollar_position + elem.dst_index);

                    set_dword_into_binout(dollar_position + elem.dst_index,
                                          binout_container, elem.rel_index);
                },
                pattern | ds(true, imm16) = [&] {
                    log()->debug("imm16: update_label_dst_offset bin[{}, {}] = 0x{:02x}",
                                 elem.rel_index,
                                 elem.rel_index + 1,
                                 dollar_position + elem.dst_index);

                    set_word_into_binout(dollar_position + elem.dst_index,
                                         binout_container, elem.rel_index);
                },
                pattern | ds(true, imm8 ) = [&] {
                    log()->debug("imm8: update_label_dst_offset bin[{}] = {}, bin[{}] = {}",
                                 elem.rel_index - 1, elem.dst_index, elem.rel_index, 0x7c);
                    binout_container[elem.rel_index - 1] = elem.dst_index;
                    binout_container[elem.rel_index] = 0x7c;
                },
                // ラベルからの相対値でオフセットを設定する
                pattern | ds(false, imm32) = [&] {
                    log()->debug("imm32: update_label_dst_offset bin[{}, {}, {}, {}] = 0x{:02x}",
                                 std::to_string(elem.rel_index),
                                 std::to_string(elem.rel_index + 1),
                                 std::to_string(elem.rel_index + 2),
                                 std::to_string(elem.rel_index + 3),
                                 elem.rel_offset());

                    set_dword_into_binout(elem.rel_offset(),
                                          binout_container,
                                          elem.rel_index);
                },
                pattern | ds(false, imm16) = [&] {
                    log()->debug("imm16: update_label_dst_offset bin[{}, {}] = 0x{:02x}",
                                 std::to_string(elem.rel_index),
                                 std::to_string(elem.rel_index + 1),
                                 elem.rel_offset() - 1);

                    set_word_into_binout(elem.rel_offset() - 1,
                                         binout_container,
                                         elem.rel_index);
                },
                pattern | ds(false, imm8 ) = [&] {
                    log()->debug("imm8: update_label_dst_offset bin[{}] = 0x{:02x}",
                                 std::to_string(elem.rel_index + 1),
                                 elem.rel_offset());
                    binout_container[elem.rel_index] = elem.rel_offset();
                }
            );
        }
    }

    bool dst_is_stored(std::string label_src) {
        LabelDstList label_dst_list = LabelDstList{};
        auto it = std::find_if(std::begin(label_dst_list), std::end(label_dst_list),
                               [&](const LabelDstElement& elem)
                               { return elem.label.find(label_src) != std::string::npos; });
        return it != std::end(label_dst_list);
    }

    void store_label_src(std::string label_src,
                         LabelSrcList& label_src_list,
                         std::vector<uint8_t>& binout_container,
                         bool abs,
                         size_t offset_size) {

        LabelSrcElement elem;
        elem.abs = abs;
        elem.offset_size = offset_size;
        elem.label = label_src;
        elem.src_index = binout_container.size();
        elem.rel_index = binout_container.size() + 1;
        label_src_list.push_back(elem);
    }

    void update_label_src_offset(std::string label_src,
                                 LabelDstList& label_dst_list,
                                 uint8_t nim,
                                 std::vector<uint8_t>& binout_container) {

        auto it = std::find_if(std::begin(label_dst_list), std::end(label_dst_list),
                               [&](const LabelDstElement& elem)
                               { return elem.label == label_src; });

        if (it != std::end(label_dst_list)) {
            LabelDstElement elem(*it);
            elem.dst_index = binout_container.size();
            elem.rel_index = binout_container.size() + 1;
            log()->debug("update_label_src_offset bin[{}] = {}",
                         std::to_string(elem.rel_index),
                         elem.rel_offset());

            binout_container.push_back(nim);
            binout_container.push_back(elem.rel_offset());
        }

        return;
    }

    // uint16_tで数値を読み取った後uint8_t型にデータを分けて、リトルエンディアンで格納する
    //
    // @param word             格納するWORDサイズのバイナリ
    // @param binout_container 出力先コンテナ
    // @param start_index      格納するコンテナの開始index
    //
    void set_word_into_binout(const uint16_t& word,
                              std::vector<uint8_t>& binout_container,
                              size_t start_index) {

        // push_back as little_endian
        const uint8_t first_byte  = (word >> 8) & 0xff;
        const uint8_t second_byte = word & 0xff;

        if (start_index == 0) {
            binout_container.push_back(second_byte);
            binout_container.push_back(first_byte);
        } else {
            binout_container.at(start_index + 0) = second_byte;
            binout_container.at(start_index + 1) = first_byte;
        }
        log()->debug("(W): 0x{:02x}, 0x{:02x}", second_byte, first_byte);
    }

    // uint32_tで数値を読み取った後、uint8_t型にデータを分けて、リトルエンディアンで格納する
    void set_dword_into_binout(const uint32_t& dword,
                               std::vector<uint8_t>& binout_container,
                               size_t start_index) {

        uint32_t cp_dword = dword;
        uint8_t bytes[4] = {};

        bytes[0] = (cp_dword >> 24) & 0xff;
        bytes[1] = (cp_dword >> 16) & 0xff;
        bytes[2] = (cp_dword >> 8)  & 0xff;
        bytes[3] = cp_dword & 0xff;

        if (start_index == 0) {
            binout_container.push_back(bytes[3]);
            binout_container.push_back(bytes[2]);
            binout_container.push_back(bytes[1]);
            binout_container.push_back(bytes[0]);
        } else {
            binout_container.at(start_index + 0) = bytes[3];
            binout_container.at(start_index + 1) = bytes[2];
            binout_container.at(start_index + 2) = bytes[1];
            binout_container.at(start_index + 3) = bytes[0];
        }
        log()->debug("(DW): 0x{:02x}, 0x{:02x}, 0x{:02x}, 0x{:02x}",
                     bytes[3], bytes[2], bytes[1], bytes[0]);
    }

    /**
    const long get_label_src_offset(std::string label_src) {

        auto it = std::find_if(std::begin(label_dst_list), std::end(label_dst_list),
                               [&](const LabelDstElement& elem)
                               { return elem.label == label_src; });

        if (it != std::end(label_dst_list)) {
            LabelDstElement elem(*it);
            log()->debug("matched label: {} with {}", elem.label, label_src);
            const long abs_size = elem.src_index + dollar_position;
            return abs_size;
        }

        return 0;
    }

    **/
}
