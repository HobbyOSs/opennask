#ifndef OBJECT_FILE_WRITER_HH
#define OBJECT_FILE_WRITER_HH

#include <asmjit/asmjit.h>
#include "nask_defs.hpp"

class ObjectFileWriter {

public:
    void write_coff(asmjit::x86::Assembler&);
};

#endif // ! OBJECT_FILE_WRITER_HH
