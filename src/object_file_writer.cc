#include <iostream>
#include <iomanip>
#include "object_file_writer.hh"


using namespace COFFI;
using namespace asmjit;


ObjectFileWriter::ObjectFileWriter() {
    writer_ = std::make_unique<coffi>();
    writer_->create(COFFI_ARCHITECTURE_PE);

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

bool ObjectFileWriter::has_extern_symbol(const std::string& symbol) {
    return std::count(extern_symbol_list.begin(), extern_symbol_list.end(), symbol) > 0;
}

void ObjectFileWriter::write_coff(asmjit::CodeHolder& code_, asmjit::x86::Assembler& a,
                                  std::map<std::string, uint32_t>& sym_table) {

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
    text->set_reloc_count(142);  // PointerToRelocatio=142(=0x8e)固定, 要はCOFFのヘッダーサイズ+2byte
    text->set_line_num_count(0); // PointerToLinenumbers=0, 使われていないので0に設定（しかし設定が効かない）

    // EXTERNで宣言されたシンボルについてcoff relocationを設定する
    for (int i = 0; i < extern_symbol_list.size(); i++) {
        auto symbol_name = extern_symbol_list[i];
        auto offset = sym_table[symbol_name];

        rel_entry_generic relocation_entry;
        std::memset(&relocation_entry, 0, sizeof(relocation_entry)); // 一度0で初期化
        relocation_entry.virtual_address = static_cast<uint32_t>(offset);
        relocation_entry.symbol_table_index = static_cast<uint32_t>(8+i);
        relocation_entry.type = IMAGE_REL_I386_REL32;

        text->add_relocation_entry(&relocation_entry);
    }

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
    auxiliary_symbol_record_5 aux_symbol_record_text;
    std::memset(&aux_symbol_record_text, 0, sizeof(auxiliary_symbol_record_5)); // 一度0で初期化
    aux_symbol_record_text.length = buf.size();       // .textのサイズを設定
    aux_symbol_record_text.number_of_relocations = extern_symbol_list.size();
    sym_text->get_auxiliary_symbols().push_back(*(auxiliary_symbol_record*)&aux_symbol_record_text);

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

    // いったんシンボルテーブル以外のWCOFFを出力する
    std::ostringstream oss;
    writer_->save(oss);

    // GLOBALのlongなシンボル用のサイズの合計値を計算しておく
    uint32_t extern_long_symbols_size = 4; // string tableの`uint32_t size`分
    std::for_each(global_symbol_list.begin(), global_symbol_list.end(), [&extern_long_symbols_size](const std::string& str) {
        if (str.size() > 8) {
            extern_long_symbols_size += str.size() + 1;
        }
    });
    auto extern_long_symbols = std::vector<std::string>{};

    // EXTERNで宣言されたシンボルを書き込む
    for (int i = 0; i < extern_symbol_list.size(); i++) {
        auto symbol_name = extern_symbol_list[i];

        if (symbol_name.size() <= 8) {
            // shortなシンボル情報を書き込む
            symbol_record sym_record;
            std::memset(&sym_record, 0, sizeof(symbol_record)); // 構造体を0で初期化
            std::strncpy(sym_record.name, extern_symbol_list[i].c_str(), sizeof(sym_record.name));
            sym_record.storage_class = IMAGE_SYM_CLASS_EXTERNAL;
            oss.write(reinterpret_cast<const char*>(&sym_record), sizeof(sym_record));
        } else {
            // longなシンボルへのオフセットを書き込む
            coff_symbol_record offs_record;
            std::memset(&offs_record, 0, sizeof(symbol_record)); // 構造体を0で初期化
            // longなシンボル名のサイズを表す32ビット整数をバイト単位で record.name フィールドに格納
            offs_record.e.e.zeroes = 0;
            offs_record.e.e.offset = extern_long_symbols_size; // 文字列テーブルのシンボル名へのオフセット
            offs_record.storage_class = IMAGE_SYM_CLASS_EXTERNAL;
            offs_record.aux_symbols_number = 0;

            oss.write(reinterpret_cast<const char*>(&offs_record), 18);

            extern_long_symbols_size += symbol_name.size() + 1; // 終端文字をプラス
            extern_long_symbols.push_back(symbol_name + '\0');
        }
    }

    // GLOBALのlongなシンボル用のサイズカウンタ
    // 4byteが初期値となり、そのサイズのバイナリが格納される
    uint32_t global_long_symbols_size = 4;
    auto global_long_symbols = std::vector<std::string>{};

    // GLOBALで宣言されたシンボルを書き込む
    for (int i = 0; i < global_symbol_list.size(); i++) {
        auto symbol_name = global_symbol_list[i];
        auto offset_between_symbols = sym_table[symbol_name] - sym_table[global_symbol_list[0]];

        if (symbol_name.size() <= 8) {
            // shortなシンボル情報を書き込む
            symbol_record sym_record;
            std::memset(&sym_record, 0, sizeof(symbol_record)); // 構造体を0で初期化
            std::strncpy(sym_record.name, global_symbol_list[i].c_str(), sizeof(sym_record.name));
            sym_record.value = offset_between_symbols;
            sym_record.section_number = 1;
            sym_record.storage_class = IMAGE_SYM_CLASS_EXTERNAL;
            oss.write(reinterpret_cast<const char*>(&sym_record), sizeof(sym_record));
        } else {
            // longなシンボルへのオフセットを書き込む
            coff_symbol_record offs_record;
            std::memset(&offs_record, 0, sizeof(symbol_record)); // 構造体を0で初期化
            // longなシンボル名のサイズを表す32ビット整数をバイト単位で record.name フィールドに格納
            offs_record.e.e.zeroes = 0;
            offs_record.e.e.offset = global_long_symbols_size; // 文字列テーブルのシンボル名へのオフセット
            offs_record.value = sym_table[symbol_name]; // ここが違っている場合pass1の数が合っていない可能性がある
            offs_record.section_number = 1;
            offs_record.storage_class = IMAGE_SYM_CLASS_EXTERNAL;
            offs_record.aux_symbols_number = 0;

            oss.write(reinterpret_cast<const char*>(&offs_record), 18);

            global_long_symbols_size += symbol_name.size() + 1; // 終端文字をプラス
            global_long_symbols.push_back(symbol_name + '\0');
        }
    }

    // string tableにlong symbolを書き込む
    // string tableは単純な構造で
    //  * uint32_t size
    //  * char value[size]
    // という構造になっているので、そのように書き込む
    const size_t string_table_size = extern_long_symbols_size;
    oss.write(reinterpret_cast<const char*>(&string_table_size), 4);

    for (auto sym : global_long_symbols) {
        oss.write(reinterpret_cast<const char*>(sym.c_str()), sym.size());
    }
    for (auto sym : extern_long_symbols) {
        oss.write(reinterpret_cast<const char*>(sym.c_str()), sym.size());
    }


    // 出力をオブジェクトファイルとして書き出す
    std::string object_file = oss.str();

    // なぜかnaskはEXTERNシンボルのみpointerToLineNumbersに設定しているのでパッチ
    if (extern_symbol_list.size() == 0) {
        std::for_each(object_file.begin() + 49, object_file.begin() + 52 + 1, [](char &c) { c = '\x00'; });
    }
    // シンボルの数を合計してCOFFオブジェクトに設定
    int all_sym_size = 8 + global_symbol_list.size() + extern_symbol_list.size();
    put4b(object_file, 12, all_sym_size);

    /** debug用
    for (int i = 0; i < object_file.size(); i++) {
        if (i % 16 == 0) {
            std::cout << std::endl;
        }

        std::cout << std::hex
                  << std::setw(2)
                  << std::setfill('0')
                  << (int)(unsigned char) object_file[i] << " ";
    }
    std::cout << std::dec << std::endl; // 16進数表示を元に戻す
    */

    // もともとasmjitのバッファに書き込まれているデータをリセットして上書きする
    memset(buf._data, 0, buf.size());
    memcpy(buf.begin(), object_file.c_str(), object_file.size());
    buf._size = object_file.size();
    a._bufferPtr = buf.data() + object_file.size(); // asmjit/src/asmjit/core/assembler.h L.40
};
