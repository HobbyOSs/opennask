#ifndef OBJECT_FILE_WRITER_HH
#define OBJECT_FILE_WRITER_HH

#include <asmjit/asmjit.h>
#include <coffi/coffi.hpp>
#include "nask_defs.hpp"

class ObjectFileWriter {

public:
    std::unique_ptr<COFFI::coffi> writer_;
    ObjectFileWriter();

    void set_file_name(std::string&);
    void write_coff(asmjit::CodeHolder&, asmjit::x86::Assembler&);
};

#endif // ! OBJECT_FILE_WRITER_HH
