#ifndef OBJECT_FILE_WRITER_HH
#define OBJECT_FILE_WRITER_HH

#include <asmjit/asmjit.h>
#include <coffi/coffi.hpp>
#include "nask_defs.hpp"

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
    void write_coff(asmjit::CodeHolder&, asmjit::x86::Assembler&);
};

#endif // ! OBJECT_FILE_WRITER_HH
