#include "mod_rm.hpp"
#include "string_util.hpp"

namespace ModRM {
    using namespace nask_utility;

    size_t get_imm_size_evenif_bracket(const std::string& hex_string) {
        if (starts_with(hex_string, "[") && ends_with(hex_string, "]")) {
            std::string reg = hex_string;
            reg = reg.erase(0, 1);
            reg = reg.erase(reg.size() - 1);
            return get_imm_size(reg);
        } else {
            return get_imm_size(hex_string);
        }
    }

    size_t get_imm_size(const std::string& hex_string) {
        if (is_hex_notation(hex_string)) {
            // サイズが偶数じゃないような場合の調整 ex) 0xffc => 0x0ffc
            const std::string supplied_hex = (hex_string.substr(2).size() % 2 == 0) ?
                hex_string.substr(2) : "0" + hex_string.substr(2);
            const size_t s = supplied_hex.size() / 2;
            log()->info("check imm size: {} => {}", hex_string, s);
            return s;
        } else {
            return 0;
        }
    }

    const std::string get_MMMSSS_from_reg(const std::string& reg) {
        if (REGISTERS_MMM_MAP.count(reg) != 0) {
            return REGISTERS_MMM_MAP.at(reg);
        } else if (SEGMENT_REGISTERS_SSS_MAP.count(reg) != 0) {
            return SEGMENT_REGISTERS_SSS_MAP.at(reg);
        } else {
            return "000";
        }
    }

    const std::string get_rm_from_reg(const std::string& src_reg) {
        log()->info("ModR/M bit detecting... {}", src_reg);
        std::smatch match;

        const size_t imm = get_imm_size_evenif_bracket(src_reg);
        if (imm == imm16) {
            return "110";
        } else if (imm == imm32) {
            return "110";
        }

        if (regex_match(src_reg, match, rm000)) {
            return "000";
        } else if (regex_match(src_reg, match, rm001)) {
            return "001";
        } else if (regex_match(src_reg, match, rm010)) {
            return "010";
        } else if (regex_match(src_reg, match, rm011)) {
            return "011";
        } else if (regex_match(src_reg, match, rm101)) {
            return "101";
        } else if (regex_match(src_reg, match, rm110)) {
            return "110";
        } else if (regex_match(src_reg, match, rm111)) {
            return "111";
        } else if (SEGMENT_REGISTERS_SSS_MAP.count(src_reg)) {
            return SEGMENT_REGISTERS_SSS_MAP.at(src_reg);
        } else if (REGISTERS_MMM_MAP.count(src_reg)) {
            return REGISTERS_MMM_MAP.at(src_reg);
        }

        // [<SIB>], [<SIB>+disp8], [<SIB>+disp32]
        return "100";
    };

    const uint8_t get_opecode_from_reg(uint8_t base, const std::string& reg_name) {
        std::tuple<std::string, std::string> tuple = REGISTERS_RRR_MAP.at(reg_name);
        std::string register_number = std::get<0>(tuple);
        const std::bitset<8> bitset("00000" + register_number);
        log()->info("+rb, +rw, +rd, +ro = 00000{}", register_number);
        const uint8_t ret = base + bitset.to_ulong();
        log()->info("0x{:02x} + 0x{:02x} = 0x{:02x}", base, bitset.to_ulong(), ret);
        return ret;
    }

    // @param m         : [mod] mods::REG_REG, REG_DISP8, REG_DISP16, REG
    // @param data_type : [reg] BYTE, WORD, DWORD
    // @param reg       : [r/m] '/x' with Enum
    uint8_t generate_modrm_imm(enum mods m, const std::string& data_type, enum reg_field reg) {
        //
        // Generate ModR/M byte with arguments
        // [mod] 2bit
        // [reg] 3bit
        // [r/m] 3bit
        //
        std::string modrm = ModRM::MOD_TO_STR.at(m);
        switch (reg) {
        case SLASH_0:
            modrm += "000";
            break;
        case SLASH_1:
            modrm += "001";
            break;
        case SLASH_2:
            modrm += "010";
            break;
        case SLASH_3:
            modrm += "011";
            break;
        case SLASH_4:
            modrm += "100";
            break;
        case SLASH_5:
            modrm += "101";
            break;
        case SLASH_6:
            modrm += "110";
            break;
        case SLASH_7:
            modrm += "111";
            break;
        }

        if (data_type == "WORD") {
            modrm += "110";
        } else if (data_type == "DWORD") {
            modrm += "101";
        }

        std::bitset<8> bs(modrm);
        log()->info("Generate ModR/M: bitset:{}", modrm);
        return bs.to_ulong();
    };

    // @param m       : [mod] mods::REG_REG, REG_DISP8, REG_DISP16, REG
    // @param dst_reg : [reg] Register with std::string
    // @param reg     : [r/m] '/x' with Enum
    uint8_t generate_modrm(enum mods m, const std::string& dst_reg, enum reg_field reg) {
        //
        // Generate ModR/M byte with arguments
        // [mod] 2bit
        // [reg] 3bit
        // [r/m] 3bit
        //
        std::string modrm = ModRM::MOD_TO_STR.at(m);
        if (reg == SLASH_R) {
            modrm += get_rm_from_reg(dst_reg);
        } else {
            switch (reg) {
            case SLASH_R:
                break;
            case SLASH_0:
                modrm += "000";
                break;
            case SLASH_1:
                modrm += "001";
                break;
            case SLASH_2:
                modrm += "010";
                break;
            case SLASH_3:
                modrm += "011";
                break;
            case SLASH_4:
                modrm += "100";
                break;
            case SLASH_5:
                modrm += "101";
                break;
            case SLASH_6:
                modrm += "110";
                break;
            case SLASH_7:
                modrm += "111";
                break;
            }
        }
        modrm += get_rm_from_reg(dst_reg);
        std::bitset<8> bs(modrm);
        log()->info("Generate ModR/M: bitset:{}", modrm);
        return bs.to_ulong();
    };

    // @param op      : opecode for detecting which register is src/dst
    // @param m       : [mod] mods::REG
    // @param src_reg : [r/m] Register with std::string
    // @param dst_reg : [reg] Register with std::string
    uint8_t generate_modrm(const uint8_t op, enum mods m, const std::string& src_reg, const std::string& dst_reg) {
        //
        // Generate ModR/M byte with arguments
        // [mod] 2bit
        // [reg] 3bit
        // [r/m] 3bit
        //
        std::bitset<8> bsl(op);
        log()->info("Generate ModR/M: d bit:{} dst:{}, src:{}", bsl[3], dst_reg, src_reg);
        std::string modrm = ModRM::MOD_TO_STR.at(m);

        //        hgfedcba
        // --------------------------------------------------------------
        //   8e = 10001110 => d=1: REG <- MOD R/M, REG is the destination
        //   01 = 00000001 => d=0: MOD R/M <- REG, REG is the source
        // --------------------------------------------------------------
        // The d bit in the opcode determines which operand is the source,
        // and which is the destination
        if (bsl[3]) { // d=0 or 1
            log()->info("d=1: REG <- MOD R/M, REG is the destination");
            modrm += get_rm_from_reg(dst_reg);
            modrm += get_rm_from_reg(src_reg);
        } else {
            log()->info("d=0: MOD R/M <- REG, REG is the source");
            modrm += get_rm_from_reg(src_reg);
            modrm += get_rm_from_reg(dst_reg);
        }
        log()->info("Generate ModR/M: bitset:{}", modrm);
        std::bitset<8> bs(modrm);
        return bs.to_ulong();
    };

    uint8_t generate_sib(const std::string& base_reg, const std::string& index_reg, int scale) {
        //
        // Generate SIB byte with arguments
        // [scale] 2bit
        // [index] 3bit
        // [base ] 3bit
        //
        log()->info("Generate SIB byte: scale:{} index:{}, base:{}", scale, index_reg, base_reg);
        const std::string sib = "00" + get_rm_from_reg(base_reg) + get_rm_from_reg(base_reg);
        std::bitset<8> bsl(sib);
        log()->info("SIB byte: {}", sib);
        return bsl.to_ulong();
    }

    bool is_accumulator(const std::string& reg) {
        std::smatch match;
        return regex_match(reg, match, rm000);
    }
};
