#ifndef MOD_RM_HPP_
#define MOD_RM_HPP_

#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include "nask_defs.hpp"

namespace nask_utility {
    namespace ModRM {

        using namespace std::regex_constants;

        //
        // see http://www.wdic.org/w/SCI/ModR/M
        //
        constexpr const char* _000 = "(AL|AX|EAX|\\[EAX]|\\[EAX\\+.*])";
        constexpr const char* _001 = "(CL|CX|ECX|\\[ECX]|\\[ECX\\+.*])";
        constexpr const char* _010 = "(DL|DX|EDX|\\[EDX]|\\[EDX\\+.*])";
        constexpr const char* _011 = "(BL|BX|EBX|\\[EBX]|\\[EBX\\+.*])";
        constexpr const char* _100 = "(AH|SP|ESP|\\[ESP]|\\[ESP\\+.*])";
        /**                   _100 = これは最後に評価                 */
        constexpr const char* _101 = "(CH|BP|EBP|\\[EBP\\+.*]\\[DI]|\\[DI\\+.*])";
        constexpr const char* _110 = "(DH|SI|ESI|\\[ESI]|\\[ESI\\+.*])";
        constexpr const char* _111 = "(BH|DI|EDI|\\[EDI]|\\[EDI\\+.*])";

        const std::regex rm000(_000, extended | icase);
        const std::regex rm001(_001, extended | icase);
        const std::regex rm010(_010, extended | icase);
        const std::regex rm011(_011, extended | icase);
        const std::regex rm100(_100, extended | icase);
        const std::regex rm101(_101, extended | icase);
        const std::regex rm110(_110, extended | icase);
        const std::regex rm111(_111, extended | icase);

        const std::regex regImm08("(AL|CL|DL|BL|AH|CH|DH|BH)", extended | icase);
        const std::regex regImm16("(AX|BX|CX|DX|SI|DI|BP|SP|IP|FLAGS|CS|SS|DS|ES|FS|GS)", extended | icase);
        const std::regex regImm32("(EAX|EBX|ECX|EDX|ESI|EDI|EBP|ESP|EIP|EFLAGS)", extended | icase);

        const std::string SIB = "100";

        enum mods {
            REG_REG = 0, // mod=00: [レジスター+レジスター]
            REG_DISP8,   // mod=01: [レジスター+disp8]
            REG_DISP16,  // mod=10: [レジスター+disp16/32]
            REG          // mod=11: レジスター
        };

        enum reg_field {
            SLASH_0 = 0, // '/0'
            SLASH_1,     // '/1'
            SLASH_2,     // '/2'
            SLASH_3,     // '/3'
            SLASH_4,     // '/4'
            SLASH_5,     // '/5'
            SLASH_6,     // '/6'
            SLASH_7,     // '/7'
            SLASH_R      // '/r'
        };

        const std::map<enum mods, std::string> MOD_TO_STR {
            { mods::REG_REG    , "00"},
            { mods::REG_DISP8  , "01"},
            { mods::REG_DISP16 , "10"},
            { mods::REG	  , "11"}
        };

        // @see: https://courses.engr.illinois.edu/ece390/resources/opcodes.html
        // rrr : W=0 : W=1 : reg32
        //------------------------
        // 000 : AL  : AX  : EAX
        // 001 : CL  : CX  : ECX
        // 010 : DL  : DX  : EDX
        // 011 : BL  : BX  : EBX
        // 100 : AH  : SP  : ESP
        // 101 : CH  : BP  : EBP
        // 110 : DH  : SI  : ESI
        // 111 : BH  : DI  : EDI
        const std::map<std::string, std::tuple<std::string, std::string>> REGISTERS_RRR_MAP {
            // reg:                [rrr,   W]
            { "AL", std::make_tuple("000", "0") },
            { "CL", std::make_tuple("001", "0") },
            { "DL", std::make_tuple("010", "0") },
            { "BL", std::make_tuple("011", "0") },
            { "AH", std::make_tuple("100", "0") },
            { "CH", std::make_tuple("101", "0") },
            { "DH", std::make_tuple("110", "0") },
            { "BH", std::make_tuple("111", "0") },
            { "AX", std::make_tuple("000", "1") },
            { "CX", std::make_tuple("001", "1") },
            { "DX", std::make_tuple("010", "1") },
            { "BX", std::make_tuple("011", "1") },
            { "SP", std::make_tuple("100", "1") },
            { "BP", std::make_tuple("101", "1") },
            { "SI", std::make_tuple("110", "1") },
            { "DI", std::make_tuple("111", "1") },
            {"EAX", std::make_tuple("000", "1") },
            {"ECX", std::make_tuple("001", "1") },
            {"EDX", std::make_tuple("010", "1") },
            {"EBX", std::make_tuple("011", "1") },
            {"ESP", std::make_tuple("100", "1") },
            {"EBP", std::make_tuple("101", "1") },
            {"ESI", std::make_tuple("110", "1") },
            {"EDI", std::make_tuple("111", "1") }
        };


        // This specify part of r/m
        // mmm : Function
        //-----------------------
        // 000 : DS:[BX+SI]:[EAX]
        // 001 : DS:[BX+DI]:[ECX]
        // 010 : SS:[BP+SI]:[EDX]
        // 011 : SS:[BP+DI]:[EBX]
        // 100 : DS:[SI]   :
        // 101 : DS:[DI]   :
        // 110 : SS:[BP]   :[ESI]
        // 111 : DS:[BX]   :[EDI]
        const std::map<std::string, std::string> REGISTERS_MMM_MAP {
            { "DS", "000"}, { "[BX+SI]", "000"}, { "[EAX]", "000"},
            { "DS", "001"}, { "[BX+DI]", "001"}, { "[ECX]", "001"},
            { "SS", "010"}, { "[BP+SI]", "010"}, { "[EDX]", "010"},
            { "SS", "011"}, { "[BP+DI]", "011"}, { "[EBX]", "011"},
            { "DS", "100"}, { "[SI]",    "100"}, // { "[]", "100"},
            { "DS", "101"}, { "[DI]",    "101"}, // { "[]", "101"},
            { "SS", "110"}, { "[BP]",    "110"}, { "[ESI]", "110"},
            { "DS", "111"}, { "[BX]",    "111"}, { "[EDI]", "111"},
        };

        // sss : Segment Register
        //-----------------------
        // 000 : ES
        // 001 : CS
        // 010 : SS
        // 011 : DS
        // 100 : FS (Only 386+)
        // 101 : GS (Only 386+)
        const std::map<std::string, std::string> SEGMENT_REGISTERS_SSS_MAP {
            { "ES", "000"},
            { "CS", "001"},
            { "SS", "010"},
            { "DS", "011"},
            { "FS", "100"},
            { "GS", "101"}
        };

        const std::string get_rm_from_reg(const std::string& src_reg);
        const uint8_t get_opecode_from_reg(uint8_t base, const std::string& reg_name);
        const std::string get_MMMSSS_from_reg(const std::string& reg);
        uint8_t generate_modrm(enum mods m, const std::string& dst_reg, enum reg_field reg = SLASH_R);
        uint8_t generate_modrm_imm(enum mods m, const std::string& data_type, enum reg_field reg);

        uint8_t generate_modrm(const uint8_t op, enum mods m, const std::string& src_reg, const std::string& dst_reg);
        uint8_t generate_sib(const std::string& base_reg, const std::string& index_reg, int scale = 0);
        bool is_accumulator(const std::string& reg);
    };
};

#endif /* MOD_RM_HPP_ */
