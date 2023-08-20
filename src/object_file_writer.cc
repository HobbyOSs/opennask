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
    // coffiでは`auxiliary_symbol_record_4` で定義される
    // https://coffi.readthedocs.io/en/latest/annotated.html
    // ちょっとしつこいコメントになるが設定値の意味も付記する
    symbol* sym1 = writer_->add_symbol(".file");
    // IMAGE_SYM_DEBUG
    // The symbol provides general type or debugging information but does not correspond to a section.
    // Microsoft tools use this setting along with .file records (storage class FILE).
    sym1->set_section_number(IMAGE_SYM_DEBUG);

    // IMAGE_SYM_CLASS_FILE
    // Used by Microsoft tools, as well as traditional COFF format, for the source-file symbol record.
    // The symbol is followed by auxiliary records that name the file.
    sym1->set_storage_class(IMAGE_SYM_CLASS_FILE);
    // シンボルはこの中では1つしかないので1
    sym1->set_aux_symbols_number(1);

    auxiliary_symbol_record_4 record;
    std::strncpy(record.file_name, file_name.c_str(), sizeof(record.file_name));
    for (size_t i = file_name.size(); i < sizeof(record.file_name) - 1; i++) {
        record.file_name[i] = '\0';
    }
    record.file_name[sizeof(record.file_name) - 1] = '\0'; // 末尾を0で埋める

    sym1->get_auxiliary_symbols().push_back(*(auxiliary_symbol_record*)&record);
}

void ObjectFileWriter::write_coff(asmjit::CodeHolder& code_, asmjit::x86::Assembler& a) {

    CodeBuffer& buf = code_.textSection()->buffer();

    // .textに最終的な機械語を差し込む
    section* text = writer_->add_section(".text");
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
