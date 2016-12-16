#ifndef BRACKET_IMPL_HPP_
#define BRACKET_IMPL_HPP_

#include "nask_utility.hpp"
#include "bracket_utility.hpp"

namespace nask_utility {

     /**
      * Osaskのアセンブラに独自にある以下のようなテキストを処理する
      * [FORMAT "WCOFF"], [FILE "xxxx.c"], [INSTRSET "i386"], [BITS 32]
      */
     int Instructions::process_token_BRACKET(TParaTokenizer& tokenizer, VECTOR_BINOUT& binout_container) {

	  for (TParaToken token = tokenizer.Next(); ; token = tokenizer.Next()) {
	       if (token.Is("[") && tokenizer.LookAhead(1).Is("FORMAT")) {
		    this->exists_section_table = true;
		    log()->info("process FORMAT as {}", tokenizer.LookAhead(2).AsString());

		    NAS_PIMAGE_FILE_HEADER header = {};
		    const std::string target = tokenizer.LookAhead(2).AsString();
		    process_format_statement(header, target, binout_container);
		    return 0;
	       } else if (token.Is("[") && tokenizer.LookAhead(1).Is("BITS")) {
		    log()->info("process BITS as {}", tokenizer.LookAhead(2).AsString());
		    if (tokenizer.LookAhead(2).IsInteger()) {
			 switch (tokenizer.LookAhead(2).AsLong()) {
			 case 16:
			      this->OPENNASK_MODES = ID_16BIT_MODE;
			      break;
			 case 32:
			      this->OPENNASK_MODES = ID_32BIT_MODE;
			      break;
			 case 64:
			      this->OPENNASK_MODES = ID_64BIT_MODE;
			      break;
			 default:
			      std::cerr << "NASK : BITS syntax error, "
					<< tokenizer.LookAhead(2).AsString()
					<< " is not supported"
					<< std::endl;
			      return 17;
			      break;
			 }
		    }
		    return 0;

	       } else if (token.Is("[") && tokenizer.LookAhead(1).Is("INSTRSET")) {

		    std::string cpu = tokenizer.LookAhead(2).AsString();
		    cpu = cpu.erase(0, 1);
		    cpu = cpu.erase(cpu.size() - 1);

		    if (SUPPORT_CPUS.count(cpu) == 0) {
			 std::cerr << "NASK : INSTRSET syntax error, "
				   << cpu
				   << " is not supported"
				   << std::endl;
			 return 17;
		    } else {
			 this->support = SUPPORT_CPUS.at(cpu.c_str());
			 log()->info("process INSTRSET as {}", this->support_cpus[this->support]);
			 return 0;
		    }

	       } else if (token.Is("[") && tokenizer.LookAhead(1).Is("FILE")) {
		    // ".file"のフィールドの書き込みは、
		    // 通常のオペコードの書き込みが終了したあとに行う必要があるようだ
		    log()->info("process {}", tokenizer.LookAhead(1).AsString());
		    log()->info("process bracket {}", tokenizer.LookAhead(2).AsString());

		    // auxiliary element ".file"
		    std::string file_name = tokenizer.LookAhead(2).AsString();
		    file_name = file_name.erase(0, 1);
		    file_name = file_name.erase(file_name.size() - 1);

		    this->file_auxiliary_name = file_name;
		    this->exists_file_auxiliary = true;
		    this->file_auxiliary = {
			 { '.', 'f', 'i', 'l', 'e', 0, 0, 0 /* shortName */ },
			 0x00000000,
			 0xfffe,
			 0x0000,
			 0x67, 0x01
		    };
	       }
	       break;
	  }

	  return 0;
     }

     void process_format_statement(NAS_PIMAGE_FILE_HEADER& header, const std::string& target, VECTOR_BINOUT& binout_container) {

	  if (target == "\"WCOFF\"") {
	       log()->info("target: {}, process as Portable Executable", target);
	       header.machine              = I386MAGIC;
	       header.numberOfSections     = 0x0003;
	       header.pointerToSymbolTable = 0x00000000; /* symboltable		   */
	       header.numberOfSymbols	   = 0x00000000; /* number of symbols 8+α */

	       auto ptr = reinterpret_cast<uint8_t*>(&header);
	       auto buffer = std::vector<uint8_t>{ ptr, ptr + sizeof(header) };
	       std::copy(buffer.begin(), buffer.end(), back_inserter(binout_container));

	       // section table ".text"
	       NAS_PIMAGE_SECTION_HEADER text = {
		    { '.', 't', 'e', 'x', 't', 0, 0, 0 /* name */ },
		    0x00000000, // Misc
		    0x00000000, // VirtualAddress
		    0x00000002,	// SizeOfRawData
		    0x0000008c,	// PointerToRawData
		    0x0000008e,	// PointerToRelocations
		    0x00000000,	// PointerToLinenumbers
		    0x0000,	// NumberOfRelocations
		    0x0000,	// NumberOfLinenumbers
		    0x60100020  /* +0x38: flags, default_align = 1 */
	       };

	       auto text_buffer = create_buffer(text);
	       std::copy(text_buffer.begin(), text_buffer.end(), back_inserter(binout_container));

	       // section table ".data"
	       NAS_PIMAGE_SECTION_HEADER data = {
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

	       auto data_buffer = create_buffer(data);
	       std::copy(data_buffer.begin(), data_buffer.end(), back_inserter(binout_container));

	       // section table ".bss"
	       NAS_PIMAGE_SECTION_HEADER bss = {
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

	       auto bss_buffer = create_buffer(bss);
	       std::copy(bss_buffer.begin(), bss_buffer.end(), back_inserter(binout_container));
	       log()->info("Wrote '.text', '.data', '.bss' fields for Portable Executable");
	  }
     }

     void process_section_table(Instructions& inst, VECTOR_BINOUT& binout_container) {

	  // COFFヘッダのシンボルテーブルへのオフセットが確定
	  log()->info("COFF file section table starts with: bin[{}]", binout_container.size());
	  log()->info("Previous 4byte for debug 0x{:02x}, 0x{:02x}, 0x{:02x}, 0x{:02x}",
		      binout_container.at(binout_container.size()-4),
		      binout_container.at(binout_container.size()-3),
		      binout_container.at(binout_container.size()-2),
		      binout_container.at(binout_container.size()-1));

	  const uint32_t offset = binout_container.size();
	  // セクションデータのサイズが確定(SizeOfRawData)
	  const uint32_t size_of_raw_data =
	       offset - (sizeof(NAS_PIMAGE_FILE_HEADER) + sizeof(NAS_PIMAGE_SECTION_HEADER) * 3);

	  // EXTERNされたシンボル名の数だけ"COFF Relocation"を書き出す
	  for ( size_t i = 0; i < inst.ex_symbol_list.size(); i++) {
	       const std::string symbol_name = inst.ex_symbol_list[i];
	       log()->info("EXTERN symbol[{}/{}]", i+1, inst.ex_symbol_list.size());

	       // ???
	       // ---
	       // Address of the item to which relocation is applied: this is the offset from the
	       // beginning of the section, plus the value of the section’s RVA/Offset field
	       // (see Section 4, “Section Table.”). For example, if the first byte of
	       // the section has an address of 0x10, the third byte has an address of 0x12.
	       // ---
	       // Type 20 - IMAGE_REL_I386_REL32 - might sound tricky first, but it is in fact quite simple.
	       // Take the virtual address B at which this relocation is applied (offset within section A1
	       // plus virtual address of section A2). Take the virtual address of the target symbol C,
	       // which is computed the same way (A1+A2 only the values are different).
	       // The relocation is C-B, this is added to whatever dword value you find at B
	       // (most of the times it will be zero). This is usually applied to "jmp near" and "call near"
	       // assembly instructions, and the way the x86 CPU works, it takes the operand of the instruction
	       // (C-B+some value) and adds it to the address of the instruction (B + some value),
	       // yielding the correct result C. See?
	       // ---
	       // VA(B)  = 320byte __HERE!__
	       // VA(C)  = ...byte
	       // v_addr = C - B = (?) - 320 => 125
	       // (?)    = 445
	       uint32_t v_addr = 0x00000000;

	       NAS_COFF_RELOCATION reloc = {
		    0x00000000,
		    static_cast<uint32_t>(8+i),
		    IMAGE_REL_I386_REL32
	       };
	       auto reloc_buffer = create_buffer(reloc);
	       std::copy(reloc_buffer.begin(), reloc_buffer.end(), back_inserter(binout_container));
	  }

	  log()->info("COFF file header's PointerToSymbolTable: 0x{:02x}", offset);
	  set_dword_into_binout(offset, binout_container, false, 8);
	  log()->info("section table '.text' PointerToSymbolTable: 0x{:02x}", offset + 4);
	  set_dword_into_binout(offset, binout_container, false, sizeof(NAS_PIMAGE_FILE_HEADER) + 24);
	  log()->info("section table '.text' SizeOfRawData: 0x{:02x}", offset);
	  set_dword_into_binout(size_of_raw_data, binout_container, false, sizeof(NAS_PIMAGE_FILE_HEADER) + 16);

	  // auxiliary element ".file"
	  if (inst.exists_file_auxiliary) {
	       auto ptr = reinterpret_cast<uint8_t*>(&inst.file_auxiliary);
	       auto buffer = std::vector<uint8_t>{ ptr, ptr + sizeof(inst.file_auxiliary) };
	       std::copy(buffer.begin(), buffer.end(), back_inserter(binout_container));

	       for ( size_t i = 0; i < 18; i++ ) {
		    if ( inst.file_auxiliary_name.size() <= i ) {
			 binout_container.push_back(0x00);
		    } else {
			 binout_container.push_back(inst.file_auxiliary_name.at(i));
		    }
	       }
	       log()->info("Wrote a '.file' auxiliary field for Portable Executable");
	  }

	  // element ".text"
	  log()->info("COFF .text symbol section table starts with: bin[{}]", binout_container.size());
	  NAS_PIMAGE_SYMBOL text = {
	       { '.', 't', 'e', 'x', 't', 0, 0, 0 /* shortName */ },
	       0x00000000,
	       WCOFF_TEXT_FIELD,
	       0x0000,
	       0x03, 0x01
	  };

	  auto text_buffer = create_buffer(text);
	  std::copy(text_buffer.begin(), text_buffer.end(), back_inserter(binout_container));
	  // セクションデータのサイズを入れる(SizeOfRawData)
	  set_dword_into_binout(size_of_raw_data, binout_container);
	  for ( size_t i = 4; i < 18; i++ ) {
	       binout_container.push_back(0x00);
	  }

	  // element ".data"
	  log()->info("COFF .data symbol section table starts with: bin[{}]", binout_container.size());
	  NAS_PIMAGE_SYMBOL data = {
	       { '.', 'd', 'a', 't', 'a', 0, 0, 0 /* shortName */ },
	       0x00000000,
	       WCOFF_DATA_FIELD,
	       0x0000,
	       0x03, 0x01
	  };

	  auto data_buffer = create_buffer(data);
	  std::copy(data_buffer.begin(), data_buffer.end(), back_inserter(binout_container));
	  for ( size_t i = 0; i < 18; i++ ) {
	       binout_container.push_back(0x00);
	  }

	  // element ".bss"
	  log()->info("COFF .bss symbol section table starts with: bin[{}]", binout_container.size());
	  NAS_PIMAGE_SYMBOL bss = {
	       { '.', 'b', 's', 's', 0, 0, 0, 0 /* shortName */ },
	       0x00000000,
	       WCOFF_BSS_FIELD,
	       0x0000,
	       0x03, 0x01
	  };

	  auto bss_buffer = create_buffer(bss);
	  std::copy(bss_buffer.begin(), bss_buffer.end(), back_inserter(binout_container));
	  for ( size_t i = 0; i < 18; i++ ) {
	       binout_container.push_back(0x00);
	  }

	  // シンボル数を確定させる
	  log()->info("COFF file header's NumberOfSymbols: 0x{:02x}", 8 + inst.gl_symbol_list.size());
	  const uint32_t number_of_symbols = 8 + inst.gl_symbol_list.size();
	  set_dword_into_binout(number_of_symbols, binout_container, false, 12);

	  // 8byteより大きい
	  std::vector<std::string> all_symbol_list;
	  all_symbol_list.resize(inst.gl_symbol_list.size());
	  std::copy(inst.gl_symbol_list.begin(),
		    inst.gl_symbol_list.end(),
		    std::back_inserter(all_symbol_list));

	  std::vector<std::string> gl_long_symbol_list;
	  std::vector<std::string> ex_long_symbol_list;
	  std::map<std::string, int> ex_long_symbol_short_names;
	  uint32_t long_symbols_size = 4; // これ自体(4byte)

	  /**
	   * EXTERNで宣言されたシンボルを書き込んでいく
	   */
	  for ( size_t i = 0; i < inst.ex_symbol_list.size(); i++) {

	       const std::string symbol_name = inst.ex_symbol_list[i];
	       const uint32_t value = inst.symbol_offsets[symbol_name];
	       log()->info("symbol[{}/{}]", i+1, inst.ex_symbol_list.size());
	       log()->info("symbol: {}, offs size: {}", inst.ex_symbol_list[i], value);

	       if (symbol_name.size() <= 8) {
		    const std::string real_symbol_name = symbol_name;
		    log()->info("write short symbol name: {}", real_symbol_name);

		    NAS_PIMAGE_SYMBOL func = {
			 { 0, 0, 0, 0, 0, 0, 0, 0 },
			 0x0000,
			 0x0000,
			 0x0000,
			 0x02, 0x00
		    };

		    std::copy(real_symbol_name.begin(), real_symbol_name.end(), func.shortName);
		    auto fn_buffer = create_buffer(func);
		    std::copy(fn_buffer.begin(), fn_buffer.end(), back_inserter(binout_container));

		    log()->info("[sName] {}", string_to_hex(std::string(reinterpret_cast<char*>(&func.shortName[0]), 8)));
		    log()->info("[value] 0x{:08}", func.value);
		    log()->info("[sNumb] 0x{:04}", func.sectionNumber);
		    log()->info("[_type] 0x{:04}", func.type);
		    log()->info("[class] 0x{:02}", func.storageClass);
		    log()->info("[axsym] 0x{:02}", func.numberOfAuxSymbols);
	       } else {
		    const std::string real_symbol_name = symbol_name;
		    log()->info("only record long symbol name: {}", real_symbol_name);

		    NAS_PIMAGE_SYMBOL func = {
			 { 0, 0, 0, 0, 0, 0, 0, 0 },
			 0x0000,
			 0x0000,
			 0x0000,
			 0x02, 0x00
		    };

		    // ここの値が確定するのはすべてのシンボルを書き込んだ後
		    func.shortName[7] = 0x99; // (long_symbols_size >> 24) & 0xff;
		    func.shortName[6] = 0x99; // (long_symbols_size >> 16) & 0xff;
		    func.shortName[5] = 0x99; // (long_symbols_size >> 8)  & 0xff;
		    func.shortName[4] = 0x99; // long_symbols_size & 0xff;
		    ex_long_symbol_short_names.insert( std::make_pair(real_symbol_name, binout_container.size() + 4) );

		    // シンボルサイズ + 1 (0x00)
		    // EXTERNのシンボルについてはここでは合計しない
		    // 多分仕様的にはそう！
		    //long_symbols_size += real_symbol_name.size() + 1;

		    auto fn_buffer = create_buffer(func);
		    std::copy(fn_buffer.begin(), fn_buffer.end(), back_inserter(binout_container));

		    log()->info("[sName] {}", string_to_hex(std::string(reinterpret_cast<char*>(&func.shortName[0]), 8)));
		    log()->info("[value] 0x{:08}", func.value);
		    log()->info("[sNumb] 0x{:04}", func.sectionNumber);
		    log()->info("[_type] 0x{:04}", func.type);
		    log()->info("[class] 0x{:02}", func.storageClass);
		    log()->info("[axsym] 0x{:02}", func.numberOfAuxSymbols);


		    ex_long_symbol_list.push_back(inst.ex_symbol_list.at(i));
	       }

	       // GLOBALのシンボルリストからEXTERNのものを抜いておく
	       log()->info("gl_symbol_list size: {}", inst.gl_symbol_list.size());
	       auto removed_itr = std::remove_if(inst.gl_symbol_list.begin(),
						 inst.gl_symbol_list.end(),
						 [&symbol_name](const std::string gl){
						      return symbol_name == gl;
						 });
	       inst.gl_symbol_list.erase(removed_itr, inst.gl_symbol_list.end());
	       log()->info("gl_symbol_list size: {}", inst.gl_symbol_list.size());

	  }

	  /**
	   * 通常のGLOBALで宣言されたシンボルを書き込んでいく
	   */
	  for ( size_t i = 0; i < inst.gl_symbol_list.size(); i++) {

	       const std::string symbol_name = inst.gl_symbol_list[i];
	       const uint32_t value = inst.symbol_offsets[symbol_name];
	       log()->info("symbol[{}/{}]", i+1, inst.gl_symbol_list.size());
	       log()->info("symbol: {}, offs size: {}", inst.gl_symbol_list[i], value);

	       if (symbol_name.size() <= 8) {
		    const std::string real_symbol_name = symbol_name;
		    log()->info("write short symbol name: {}", real_symbol_name);

		    NAS_PIMAGE_SYMBOL func = {
			 { 0, 0, 0, 0, 0, 0, 0, 0 },
			 value,
			 WCOFF_TEXT_FIELD,
			 0x0000,
			 0x02, 0x00
		    };

		    std::copy(real_symbol_name.begin(), real_symbol_name.end(), func.shortName);
		    auto fn_buffer = create_buffer(func);
		    std::copy(fn_buffer.begin(), fn_buffer.end(), back_inserter(binout_container));

		    log()->info("[sName] {}", string_to_hex(std::string(reinterpret_cast<char*>(&func.shortName[0]), 8)));
		    log()->info("[value] 0x{:08}", func.value);
		    log()->info("[sNumb] 0x{:04}", func.sectionNumber);
		    log()->info("[_type] 0x{:04}", func.type);
		    log()->info("[class] 0x{:02}", func.storageClass);
		    log()->info("[axsym] 0x{:02}", func.numberOfAuxSymbols);
	       } else {
		    const std::string real_symbol_name = symbol_name;
		    log()->info("only record long symbol name: {}", real_symbol_name);

		    NAS_PIMAGE_SYMBOL func = {
			 { 0, 0, 0, 0, 0, 0, 0, 0 },
			 value,
			 WCOFF_TEXT_FIELD,
			 0x0000,
			 0x02, 0x00
		    };

		    func.shortName[7] = (long_symbols_size >> 24) & 0xff;
		    func.shortName[6] = (long_symbols_size >> 16) & 0xff;
		    func.shortName[5] = (long_symbols_size >> 8)  & 0xff;
		    func.shortName[4] = long_symbols_size & 0xff;
		    log()->info("[sName] {}", string_to_hex(std::string(reinterpret_cast<char*>(&func.shortName[0]), 8)));

		    // シンボルサイズ + 1 (0x00)
		    long_symbols_size += real_symbol_name.size() + 1;

		    auto fn_buffer = create_buffer(func);
		    std::copy(fn_buffer.begin(), fn_buffer.end(), back_inserter(binout_container));
		    gl_long_symbol_list.push_back(inst.gl_symbol_list.at(i));
	       }
	  }
	  //
	  // ("long symbol" + 終端文字) * "long symbol"の数 + DWORDサイズ = をここに書き込む
	  //

	  // EXTERNのシンボルのオフセットがやっと設定できる
	  size_t sum = 0;
	  for (const auto& kv : ex_long_symbol_short_names) {
	       log()->info("{} has value {}", kv.first, kv.second);
	       set_dword_into_binout(long_symbols_size + sum, binout_container, false, kv.second);
	       sum += kv.first.size() + 1; // symbol + 0x00
	  }

	  // ここでEXTERNなシンボルのサイズを足す
	  std::for_each(inst.ex_symbol_list.begin(),
			inst.ex_symbol_list.end(),
			[&long_symbols_size](const std::string ex_symbol) -> size_t {
			     long_symbols_size += (ex_symbol.size() <= 8) ? 0 : ex_symbol.size() + 1;
			});
	  set_dword_into_binout(long_symbols_size, binout_container);

	  // long symbolを書き込む
	  for ( const std::string symbol : all_symbol_list ) {
	       if ( std::find( gl_long_symbol_list.begin(), gl_long_symbol_list.end() , symbol ) != gl_long_symbol_list.end() ||
		    std::find( ex_long_symbol_list.begin(), ex_long_symbol_list.end() , symbol ) != ex_long_symbol_list.end()) {
		    // GL/EX いずれかの long symbolのリストにあれば書き出す
		    std::string symbol_name_hex = string_to_hex_no_notate(symbol);
		    const std::string symbols = trim(symbol_name_hex);
		    log()->info("write long symbol name: {}", symbol);
		    set_hexstring_into_binout(symbols, binout_container, false);
		    binout_container.push_back(0x00);
	       }
	  }
     }

     //
     // FIXME: このへんtemplateでなんとかならんのか
     //
     std::vector<uint8_t> create_buffer(NAS_PIMAGE_SYMBOL& symbol) {
	  auto ptr = reinterpret_cast<uint8_t*>(&symbol);
	  auto buffer = std::vector<uint8_t>{ ptr, ptr + sizeof(symbol) };
	  return buffer;
     }

     std::vector<uint8_t> create_buffer(NAS_PIMAGE_SECTION_HEADER& symbol) {
	  auto ptr = reinterpret_cast<uint8_t*>(&symbol);
	  auto buffer = std::vector<uint8_t>{ ptr, ptr + sizeof(symbol) };
	  return buffer;
     }

     std::vector<uint8_t> create_buffer(NAS_COFF_RELOCATION& symbol) {
	  auto ptr = reinterpret_cast<uint8_t*>(&symbol);
	  auto buffer = std::vector<uint8_t>{ ptr, ptr + sizeof(symbol) };
	  return buffer;
     }
}

#endif /* BRACKET_IMPL_HPP_ */
