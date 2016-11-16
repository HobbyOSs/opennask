#ifndef BRACKET_UTILITY_
#define BRACKET_UTILITY_

namespace nask_utility {

     // PE file header
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

     // PE section header
     constexpr size_t IMAGE_SIZEOF_SHORT_NAME = 8;

     struct PIMAGE_SECTION_HEADER {
	  uint8_t         name[IMAGE_SIZEOF_SHORT_NAME];
	  union {
	       uint32_t   physicalAddress;
	       uint32_t   virtualSize;
	  } misc;
	  uint32_t        virtualAddress;
	  uint32_t        sizeOfRawData;
	  uint32_t        pointerToRawData;
	  uint32_t        pointerToRelocations;
	  uint32_t        pointerToLinenumbers;
	  uint16_t        numberOfRelocations;
	  uint16_t        numberOfLinenumbers;
	  uint32_t        characteristics;
     } __attribute__((packed));

     void process_format_statement(PIMAGE_FILE_HEADER& header,
				   const std::string& target,
				   VECTOR_BINOUT& binout_container);
};

#endif /* BRACKET_UTILITY_ */
