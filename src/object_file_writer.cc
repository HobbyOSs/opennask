#include <iostream>
#include <iomanip>
#include "object_file_writer.hh"


using namespace COFFI;
using namespace asmjit;


ObjectFileWriter::ObjectFileWriter() {
    writer_ = std::make_unique<coffi>();
    writer_->create(COFFI_ARCHITECTURE_PE);
    writer_->get_header()->set_flags(IMAGE_FILE_32BIT_MACHINE);

    global_symbol_list = {};
    extern_symbol_list = {};
}

ObjectFileWriter::~ObjectFileWriter() {
    global_symbol_list.clear();
    extern_symbol_list.clear();
}

void ObjectFileWriter::set_file_name(const std::string& file_name) {
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

void ObjectFileWriter::add_global_symbol(const std::string& symbol) {
    global_symbol_list.push_back(symbol);
}

void ObjectFileWriter::add_extern_symbol(const std::string& symbol) {
    extern_symbol_list.push_back(symbol);
}

void ObjectFileWriter::write_coff(asmjit::CodeHolder& code_, asmjit::x86::Assembler& a) {

    CodeBuffer& buf = code_.textSection()->buffer();

    /**
     * section header
     */
    section* text = writer_->add_section(".text");
    text->set_flags(IMAGE_SCN_CNT_CODE    | // The section contains executable code
                    IMAGE_SCN_MEM_READ    |
                    IMAGE_SCN_MEM_EXECUTE |
                    IMAGE_SCN_ALIGN_1BYTES);
    // .textに最終的な機械語を差し込む
    text->set_data(reinterpret_cast<const char*>(buf.data()), buf.size());


    section* data = writer_->add_section(".data");
    data->set_flags(IMAGE_SCN_CNT_INITIALIZED_DATA | // The section contains initialized data.
                    IMAGE_SCN_MEM_WRITE            |
                    IMAGE_SCN_MEM_READ             |
                    IMAGE_SCN_ALIGN_1BYTES);

    section* bss = writer_->add_section(".bss");
    bss->set_flags(IMAGE_SCN_CNT_UNINITIALIZED_DATA | // The section contains uninitialized data.
                   IMAGE_SCN_MEM_WRITE              |
                   IMAGE_SCN_MEM_READ               |
                   IMAGE_SCN_ALIGN_1BYTES);

    /**
     * section tables
     */
    symbol* sym_text = writer_->add_symbol(".text");
    sym_text->set_type(IMAGE_SYM_TYPE_NOT_FUNCTION);
    sym_text->set_storage_class(IMAGE_SYM_CLASS_STATIC);
    sym_text->set_section_number(1);
    sym_text->set_aux_symbols_number(1);
    auxiliary_symbol_record_5 empty_aux_text;
    std::memset(&empty_aux_text, 0, sizeof(auxiliary_symbol_record_5)); // naskでも使われていないようなので構造体を0で初期化
    sym_text->get_auxiliary_symbols().push_back(*(auxiliary_symbol_record*)&empty_aux_text);

    symbol* sym_data = writer_->add_symbol(".data");
    sym_data->set_type(IMAGE_SYM_TYPE_NOT_FUNCTION);
    sym_data->set_storage_class(IMAGE_SYM_CLASS_STATIC);
    sym_data->set_section_number(2);
    sym_data->set_aux_symbols_number(1);
    auxiliary_symbol_record_5 empty_aux_data;
    std::memset(&empty_aux_data, 0, sizeof(auxiliary_symbol_record_5)); // naskでも使われていないようなので構造体を0で初期化
    sym_data->get_auxiliary_symbols().push_back(*(auxiliary_symbol_record*)&empty_aux_data);

    symbol* sym_bss = writer_->add_symbol(".bss");
    sym_bss->set_type(IMAGE_SYM_TYPE_NOT_FUNCTION);
    sym_bss->set_storage_class(IMAGE_SYM_CLASS_STATIC);
    sym_bss->set_section_number(3);
    sym_bss->set_aux_symbols_number(1);
    auxiliary_symbol_record_5 empty_aux_bss;
    std::memset(&empty_aux_bss, 0, sizeof(auxiliary_symbol_record_5)); // naskでも使われていないようなので構造体を0で初期化
    sym_bss->get_auxiliary_symbols().push_back(*(auxiliary_symbol_record*)&empty_aux_bss);

    // シンボル情報を書き込む
    for (auto gl_symbol : global_symbol_list) {
        symbol* sym = writer_->add_symbol(gl_symbol);
        sym->set_type(IMAGE_SYM_TYPE_FUNCTION);
        sym->set_storage_class(IMAGE_SYM_CLASS_EXTERNAL);
        sym->set_section_number(1); // .text
        sym->set_aux_symbols_number(0);
    }

    // WCOFFを出力する
    std::ostringstream oss;
    writer_->save(oss);
    std::string hex_dump = oss.str();

    // debug用
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


    // もともとasmjitのバッファに書き込まれているデータをリセットして上書きする
    memset(buf._data, 0, buf.size());
    memcpy(buf.begin(), hex_dump.c_str(), hex_dump.size());
    buf._size = hex_dump.size();
    a._bufferPtr = buf.data() + hex_dump.size(); // asmjit/src/asmjit/core/assembler.h L.40
};