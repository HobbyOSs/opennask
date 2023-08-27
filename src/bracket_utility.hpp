#ifndef BRACKET_UTILITY_
#define BRACKET_UTILITY_

#include "nask_parse_common.hh"
#include "old_nask_defs.hpp"

namespace nask_utility {

    void process_format_statement(NAS_PIMAGE_FILE_HEADER& header,
                                  const std::string& target,
                                  VECTOR_BINOUT& binout_container);

    void process_section_table(Instructions& inst, VECTOR_BINOUT& binout_container);

    std::vector<uint8_t> create_buffer(NAS_PIMAGE_SYMBOL& symbol);
    std::vector<uint8_t> create_buffer(NAS_PIMAGE_SECTION_HEADER& symbol);
    std::vector<uint8_t> create_buffer(NAS_COFF_RELOCATION& symbol);
};

#endif /* BRACKET_UTILITY_ */
