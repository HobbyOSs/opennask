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

		    IMAGE_DOS_HEADER* header = {};
		    const std::string target = tokenizer.LookAhead(2).AsString();
		    process_format_statement(header, target);

		    std::array<uint8_t, sizeof(IMAGE_DOS_HEADER)> buffer = {};
		    log()->info("sizeof(IMAGE_DOS_HEADER) {}", sizeof(IMAGE_DOS_HEADER));

		    memcpy(buffer.data(), &header, sizeof(IMAGE_DOS_HEADER));
		    for ( uint8_t x : buffer ) {
			 log()->info("process bracket 0x{:02x}", x);
		    }
		    std::copy(buffer.begin(), buffer.end(), back_inserter(binout_container));
	       }
	       break;
	  }

	  return 0;
     }

     void process_format_statement(IMAGE_DOS_HEADER* header, const std::string& target) {
	  if (target == "WCOFF") {
	       header->e_magic = 0x9999;
	  }
     }
}

#endif /* BRACKET_IMPL_HPP_ */
