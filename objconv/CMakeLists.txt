message(STATUS "Entering directory .")
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(objconv CXX)

set(objconv_VERSION 2.43.0)
set(PACKAGE ON CACHE STRING "objconv" FORCE)
set(PACKAGE_NAME ON CACHE STRING "objconv" FORCE)
set(PACKAGE_STRING ON CACHE STRING "objconv 2.43.0" FORCE)
set(PACKAGE_TARNAME ON CACHE STRING "objconv" FORCE)
set(PACKAGE_VERSION ON CACHE STRING "2.43.0" FORCE)

set(objconv_SRCS
    cmdline.cpp
    cmdline.h
    cof2asm.cpp
    cof2cof.cpp
    cof2elf.cpp
    cof2omf.cpp
    coff.cpp
    coff.h
    containers.cpp
    containers.h
    converters.h
    disasm.h
    disasm1.cpp
    disasm2.cpp
    elf.cpp
    elf.h
    elf2asm.cpp
    elf2cof.cpp
    elf2elf.cpp
    elf2mac.cpp
    error.cpp
    error.h
    library.cpp
    library.h
    mac2asm.cpp
    mac2elf.cpp
    mac2mac.cpp
    macho.cpp
    macho.h
    main.cpp
    maindef.h
    omf.cpp
    omf.h
    omf2asm.cpp
    omf2cof.cpp
    omfhash.cpp
    opcodes.cpp
    stdafx.cpp
    stdafx.h)

add_executable(objconv ${objconv_SRCS})
set_property(TARGET objconv PROPERTY CXX_STANDARD 11)
set_property(TARGET objconv PROPERTY CXX_STANDARD_REQUIRED ON)
