#include <iostream>
#include <iomanip>
#include "object_file_writer.hh"
#include <coffi/coffi.hpp>

using namespace COFFI;

void ObjectFileWriter::write_coff(asmjit::x86::Assembler& a) {

    coffi writer;

    writer.create(COFFI_ARCHITECTURE_PE);
    writer.get_header()->set_flags(IMAGE_FILE_32BIT_MACHINE);
    writer.get_header()->set_sections_count(3);

    // sectionを設定する
    section* text = writer.add_section(".text");

    // 下記のように最終的なバイナリを差し込める
    //char text[] = {
    //    '\x55',                                 // push   %ebp
    //    '\x89', '\xE5',                         // mov    %esp,%ebp
    //    '\x83', '\xE4', '\xF0',                 // and    $0xfffffff0,%esp
    //    '\xE8', '\x00', '\x00', '\x00', '\x00', // call   b <_main+0xb>
    //    '\xB8', '\x2A', '\x00', '\x00', '\x00', // mov    $0x2a,%eax
    //    '\xC9',                                 // leave
    //    '\xC3',                                 // ret
    //    '\x90',                                 // nop
    //    '\x90',                                 // nop
    //};
    //text_sec->set_data(text, sizeof(text));
    section* data = writer.add_section(".data");
    section* bss = writer.add_section(".bss");

    // WCOFFのヘッダーを出力する
    coff_header* header = writer.get_header();
    std::ostringstream oss;
    header->save(oss);

    std::string hex_dump = oss.str();
    //for (char c : hex_dump) {
    //    std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)(unsigned char)c << " ";
    //}
    //std::cout << std::dec << std::endl; // 16進数表示を元に戻す
    a.embed(hex_dump.c_str(), hex_dump.size());
};
