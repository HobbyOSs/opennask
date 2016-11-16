#ifndef BRACKET_UTILITY_
#define BRACKET_UTILITY_

namespace nask_utility {

     constexpr uint16_t I386MAGIC = 0x14c;

     struct PIMAGE_FILE_HEADER {
	  uint16_t  machine;
	  uint16_t  numberOfSections;
	  int32_t   timeDateStamp;
	  int32_t   pointerToSymbolTable;
	  int32_t   numberOfSymbols;
	  uint16_t  sizeOfOptionalHeader;
	  uint16_t  characteristics;
     } __attribute__((packed));

     void process_format_statement(PIMAGE_FILE_HEADER& header, const std::string& target);
};

#endif /* BRACKET_UTILITY_ */
