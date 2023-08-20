#include <iostream>
#include <iomanip>
#include "object_file_writer.hh"


using namespace COFFI;
using namespace asmjit;


ObjectFileWriter::ObjectFileWriter() {
    writer_ = std::make_unique<coffi>();
    writer_->create(COFFI_ARCHITECTURE_PE);

    // TODO: なぜかoptional headerをつけるとマジックバイトが0x4d5a(=exe)になってしまう
    // writer_->create_optional_header();
    writer_->get_header()->set_flags(IMAGE_FILE_32BIT_MACHINE);

}


void ObjectFileWriter::set_file_name(std::string& file_name) {
    //writer->();
}

void ObjectFileWriter::write_coff(asmjit::CodeHolder& code_, asmjit::x86::Assembler& a) {

    CodeBuffer& buf = code_.textSection()->buffer();

    // .textに最終的な機械語を差し込む
    section* text = writer_->add_section(".text");
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
    text->set_data(reinterpret_cast<const char*>(buf.data()), buf.size());
    section* data = writer_->add_section(".data");
    section* bss = writer_->add_section(".bss");

    // WCOFFを出力する
    std::ostringstream oss;
    writer_->save(oss);
    std::string hex_dump = oss.str();

    for (int i = 0; i < hex_dump.size(); i++) {
        if (i % 16 == 0) {
            std::cout << std::endl;
        }

        std::cout << std::hex
                  << std::setw(2)
                  << std::setfill('0')
                  << (int)(unsigned char) hex_dump[i] << " ";
    }
    std::cout << std::dec << std::endl; // 16進数表示を元に戻す

    a.embed(hex_dump.c_str(), hex_dump.size());
};
