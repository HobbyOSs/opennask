#ifndef BRACKET_IMPL_HPP_
#define BRACKET_IMPL_HPP_

#include "nask_utility.hpp"
#include "bracket_utility.hpp"

namespace nask_utility {

     int Instructions::process_token_BRACKET(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {

	  for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
	       if (token.Is("[") && tokenizer.LookAhead(1).AsString() == "FORMAT") {
		    log()->info("process {}", tokenizer.LookAhead(1).AsString());
		    log()->info("process bracket {}", tokenizer.LookAhead(2).AsString());

		    PIMAGE_FILE_HEADER header = {};
		    const std::string target = tokenizer.LookAhead(2).AsString();
		    process_format_statement(header, target, binout_container);

	       } else if (token.Is("[") && tokenizer.LookAhead(1).AsString() == "FILE") {
		    log()->info("process {}", tokenizer.LookAhead(1).AsString());
		    log()->info("process bracket {}", tokenizer.LookAhead(2).AsString());

		    std::string file_name = tokenizer.LookAhead(2).AsString();
		    file_name = file_name.erase(0, 1);
		    file_name = file_name.erase(file_name.size() - 1);

		    const uint32_t value = std::atoi(file_name.c_str());

		    // auxiliary element ".file"
		    PIMAGE_SYMBOL file = {
			 { '.', 'f', 'i', 'l', 'e', 0, 0, 0 /* name */ },
			 value
		    };

		    auto ptr = reinterpret_cast<uint8_t*>(&file);
		    auto buffer = std::vector<uint8_t>{ ptr, ptr + sizeof(file) };
		    std::copy(buffer.begin(), buffer.end(), back_inserter(binout_container));

		    log()->info("Wrote a '.file' auxiliary field for Portable Executable");
	       }
	       break;
	  }

	  return 0;
     }

     void process_format_statement(PIMAGE_FILE_HEADER& header, const std::string& target, VECTOR_BINOUT& binout_container) {

	  if (target == "\"WCOFF\"") {
	       log()->info("target: {}, process as Portable Executable", target);
	       header.machine              = I386MAGIC;
	       header.numberOfSections     = 0x0003;
	       header.pointerToSymbolTable = 0x0000008e; /* +0x08: symboltable */
	       header.numberOfSymbols      = 0x00000009; /* +0x0c: sizeof (symboltable) / 18 */

	       auto ptr = reinterpret_cast<uint8_t*>(&header);
	       auto buffer = std::vector<uint8_t>{ ptr, ptr + sizeof(header) };
	       std::copy(buffer.begin(), buffer.end(), back_inserter(binout_container));

	       // section table ".text"
	       PIMAGE_SECTION_HEADER text = {
		    { '.', 't', 'e', 'x', 't', 0, 0, 0 /* name */ },
		    0x00000000, // Misc
		    0x00000000, // VirtualAddress
		    0x00000002,	// SizeOfRawData
		    0x0000008c,	// PointerToRawData
		    0x0000008e,	// PointerToRelocations <-- 可変
		    0x00000000,	// PointerToLinenumbers
		    0x0000,	// NumberOfRelocations
		    0x0000,	// NumberOfLinenumbers
		    0x60100020  /* +0x38: flags, default_align = 1 */
	       };

	       auto text_ptr = reinterpret_cast<uint8_t*>(&text);
	       auto text_buffer = std::vector<uint8_t>{ text_ptr, text_ptr + sizeof(text) };
	       std::copy(text_buffer.begin(), text_buffer.end(), back_inserter(binout_container));

	       // section table ".data"
	       PIMAGE_SECTION_HEADER data = {
		    { '.', 'd', 'a', 't', 'a', 0, 0, 0 /* name */ },
		    0x00000000, // Misc
		    0x00000000, // VirtualAddress
		    0x00000000,	// SizeOfRawData
		    0x00000000,	// PointerToRawData
		    0x00000000,	// PointerToRelocations
		    0x00000000,	// PointerToLinenumbers
		    0x0000,	// NumberOfRelocations
		    0x0000,	// NumberOfLinenumbers
		    0xc0100040  /* +0x60: flags, default_align = 1 */
	       };

	       auto data_ptr = reinterpret_cast<uint8_t*>(&data);
	       auto data_buffer = std::vector<uint8_t>{ data_ptr, data_ptr + sizeof(data) };
	       std::copy(data_buffer.begin(), data_buffer.end(), back_inserter(binout_container));

	       // section table ".bss"
	       PIMAGE_SECTION_HEADER bss = {
		    { '.', 'b', 's', 's', 0, 0, 0, 0 /* name */ },
		    0x00000000, // Misc
		    0x00000000, // VirtualAddress
		    0x00000000,	// SizeOfRawData
		    0x00000000,	// PointerToRawData
		    0x00000000,	// PointerToRelocations
		    0x00000000,	// PointerToLinenumbers
		    0x0000,	// NumberOfRelocations
		    0x0000,	// NumberOfLinenumbers
		    0xc0100080  /* +0x88: flags, default_align = 1 */
	       };

	       auto bss_ptr = reinterpret_cast<uint8_t*>(&bss);
	       auto bss_buffer = std::vector<uint8_t>{ bss_ptr, bss_ptr + sizeof(bss) };
	       std::copy(bss_buffer.begin(), bss_buffer.end(), back_inserter(binout_container));
	       log()->info("Wrote '.text', '.data', '.bss' fields for Portable Executable");

	  }
     }
}

#endif /* BRACKET_IMPL_HPP_ */
