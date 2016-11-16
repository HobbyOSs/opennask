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
	       header.pointerToSymbolTable = 0x0000008e;
	       header.numberOfSymbols      = 0x00000009;

	       auto ptr = reinterpret_cast<uint8_t*>(&header);
	       auto buffer = std::vector<uint8_t>{ ptr, ptr + sizeof(header) };
	       std::copy(buffer.begin(), buffer.end(), back_inserter(binout_container));

	       // section table ".text"
	       PIMAGE_SECTION_HEADER text = {
		    0x2e, 0x74, 0x65, 0x78, 0x74, 0x00, 0x00, 0x00,
		    0x00000000, // unionの分
		    0x00000000,
		    0x00000002,
		    0x0000008c,
		    0x0000008e,
		    0x00000000,
		    0x0000,
		    0x0000,
		    0x60100020
	       };

	       auto text_ptr = reinterpret_cast<uint8_t*>(&text);
	       auto text_buffer = std::vector<uint8_t>{ text_ptr, text_ptr + sizeof(text) };
	       std::copy(text_buffer.begin(), text_buffer.end(), back_inserter(binout_container));
	  }
     }
}

#endif /* BRACKET_IMPL_HPP_ */
