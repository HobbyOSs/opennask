#ifndef OBJECT_FILE_WRITER_HH
#define OBJECT_FILE_WRITER_HH

#include <asmjit/asmjit.h>
#include <coffi/coffi.hpp>
#include "nask_defs.hpp"

// http://www.delorie.com/djgpp/doc/coff/symtab.html
typedef struct {
    union {
        char name[8];        // シンボル名が8byte以下の場合、このフィールドに保存される
        struct {             // nameは長さが8byte未満の場合にのみnullで終了し、それ以外の場合はnullで終了しない
            uint32_t zeroes; // 名前がインライン化されているかどうかを示すフラグ(if=0ならばoffsetにstring tableへのオフセットが入る)
            uint32_t offset; // zeroesがゼロ の場合、このフィールドには文字列テーブル内のシンボル名へのオフセットが含まれる
        } e;
    } e;
    uint32_t value;          // シンボルの値, シンボルのタイプで中身は変わる
    uint16_t section_number; // セクション番号
    uint16_t type;           // シンボルのタイプ
    uint8_t storage_class;   // ストレージクラス
    uint8_t aux_symbols_number; // 補助エントリの数
} coff_symbol_record;


class ObjectFileWriter {

    std::vector<std::string> global_symbol_list;
    std::vector<std::string> extern_symbol_list;

public:
    std::unique_ptr<COFFI::coffi> writer_;
    ObjectFileWriter();
    ~ObjectFileWriter();

    void set_file_name(const std::string&);
    void add_global_symbol(const std::string&);
    void add_extern_symbol(const std::string&);
    void write_coff(asmjit::CodeHolder&, asmjit::x86::Assembler&, std::map<std::string, uint32_t>&);
};

#endif // ! OBJECT_FILE_WRITER_HH
