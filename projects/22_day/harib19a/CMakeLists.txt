#----------------------------------------------------------
message(STATUS "Entering directory projects/22_day/harib19a/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(22_day_harib19a_OS    ${root_BINARY_DIR}/projects/22_day/harib19a/os.img)
set(22_day_harib19a_SYS	  ${root_BINARY_DIR}/projects/22_day/harib19a/os.sys)
set(22_day_harib19a_IPLB  ${root_BINARY_DIR}/projects/22_day/harib19a/ipl.bin)
set(22_day_harib19a_IPLS  ${root_SOURCE_DIR}/projects/22_day/harib19a/ipl10.nas)
set(22_day_harib19a_HEADB ${root_BINARY_DIR}/projects/22_day/harib19a/asmhead.bin)
set(22_day_harib19a_HEADS ${root_SOURCE_DIR}/projects/22_day/harib19a/asmhead.nas)
set(22_day_harib19a_BOOTB ${root_BINARY_DIR}/projects/22_day/harib19a/boot.bin)
set(22_day_harib19a_CCO	  ${root_BINARY_DIR}/projects/22_day/harib19a/boot.o)
file(GLOB 22_day_harib19a_CCS ${root_SOURCE_DIR}/projects/22_day/harib19a/*.c)
list(REMOVE_ITEM 22_day_harib19a_CCS
  ${root_SOURCE_DIR}/projects/22_day/harib19a/a.c
  ${root_SOURCE_DIR}/projects/22_day/harib19a/hello3.c
  ${root_SOURCE_DIR}/projects/22_day/harib19a/crack1.c)
set(22_day_harib19a_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(22_day_harib19a_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(22_day_harib19a_FUNCS ${root_SOURCE_DIR}/projects/22_day/harib19a/naskfunc.nas)
set(22_day_harib19a_FUNCO ${root_BINARY_DIR}/projects/22_day/harib19a/naskfunc.o)
set(22_day_harib19a_FONTS ${root_SOURCE_DIR}/projects/22_day/harib19a/hankaku.txt)
set(22_day_harib19a_FONTB ${root_BINARY_DIR}/projects/22_day/harib19a/hankaku.bin)
set(22_day_harib19a_FONTO ${root_BINARY_DIR}/projects/22_day/harib19a/hankaku.o)
set(22_day_harib19a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(22_day_harib19a_LIBGC ${root_BINARY_DIR}/projects/22_day/harib19a/libgo.o)
set(22_day_harib19a_WILDOBJ ${root_BINARY_DIR}/projects/22_day/harib19a/*.o)


add_custom_target(22_day_harib19a_run
  COMMAND ${QEMU} ${QEMUOPT} ${22_day_harib19a_OS}
  DEPENDS 22_day_harib19a_img
)
add_custom_target(22_day_harib19a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${22_day_harib19a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 22_day_harib19a_img
)
add_custom_target(22_day_harib19a_clean
  COMMAND rm -f ${22_day_harib19a_OS}
  COMMAND rm -f ${22_day_harib19a_SYS}
  COMMAND rm -f ${22_day_harib19a_IPLB}
  COMMAND rm -f ${22_day_harib19a_HEADB}
  COMMAND rm -f ${22_day_harib19a_BOOTB}
  COMMAND rm -f ${22_day_harib19a_WILDOBJ}
)
add_custom_target(22_day_harib19a_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${22_day_harib19a_IPLS} ${22_day_harib19a_IPLB}
  DEPENDS ${22_day_harib19a_IPLS}
)
add_custom_command(
  OUTPUT ${22_day_harib19a_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${22_day_harib19a_HEADS} ${22_day_harib19a_HEADB}
  DEPENDS ${22_day_harib19a_HEADS}
  COMMENT "Generating asmhead.bin for 22_day_harib19a"
)
add_custom_command(
  OUTPUT ${22_day_harib19a_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${22_day_harib19a_FUNCS} ${22_day_harib19a_FUNCO}
  DEPENDS ${22_day_harib19a_FUNCS}
  COMMENT "Generating naskfunc.o for 22_day_harib19a"
)
add_custom_command(
  OUTPUT ${22_day_harib19a_FONTO}
  COMMAND ${FONT} ${22_day_harib19a_FONTS} ${22_day_harib19a_FONTB}
  COMMAND ${B2O}  ${22_day_harib19a_FONTB} ${22_day_harib19a_FONTO} _hankaku
  DEPENDS ${22_day_harib19a_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 22_day_harib19a"
)
add_custom_command(
  OUTPUT ${22_day_harib19a_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${22_day_harib19a_LIBGE} ${22_day_harib19a_LIBGC}
  DEPENDS ${22_day_harib19a_LIBGE}
  COMMENT "Converting golibc for 22_day_harib19a"
)

add_custom_target(22_day_harib19a_sys
  COMMAND gcc ${BINOPT} -T ${22_day_harib19a_LDS} ${22_day_harib19a_CCS} ${22_day_harib19a_WILDOBJ} -o ${22_day_harib19a_BOOTB}
  COMMAND cat ${22_day_harib19a_HEADB} ${22_day_harib19a_BOOTB} > ${22_day_harib19a_SYS}
  DEPENDS ${22_day_harib19a_HEADB} 22_day_harib19a_ipl
  DEPENDS ${22_day_harib19a_FUNCO}
  DEPENDS ${22_day_harib19a_FONTO} ${22_day_harib19a_LIBGC}
)
add_custom_target(22_day_harib19a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${22_day_harib19a_IPLB} -i ${22_day_harib19a_OS}
  COMMAND mcopy -i ${22_day_harib19a_OS} ${22_day_harib19a_SYS} ::
  COMMAND mcopy -i ${22_day_harib19a_OS} ${root_BINARY_DIR}/projects/22_day/harib19a/a.hrb ::a.hrb
  COMMAND mcopy -i ${22_day_harib19a_OS} ${root_BINARY_DIR}/projects/22_day/harib19a/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${22_day_harib19a_OS} ${root_BINARY_DIR}/projects/22_day/harib19a/crack1.hrb ::crack1.hrb
  COMMAND mcopy -i ${22_day_harib19a_OS} ${root_BINARY_DIR}/projects/22_day/harib19a/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${22_day_harib19a_OS} ${root_BINARY_DIR}/projects/22_day/harib19a/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${22_day_harib19a_OS} ${root_BINARY_DIR}/projects/22_day/harib19a/crack2.hrb ::crack2.hrb
  COMMAND mcopy -i ${22_day_harib19a_OS} ${root_BINARY_DIR}/projects/22_day/harib19a/crack3.hrb ::crack3.hrb
  COMMAND mcopy -i ${22_day_harib19a_OS} ${root_BINARY_DIR}/projects/22_day/harib19a/crack4.hrb ::crack4.hrb
  COMMAND mcopy -i ${22_day_harib19a_OS} ${root_BINARY_DIR}/projects/22_day/harib19a/crack5.hrb ::crack5.hrb
  COMMAND mcopy -i ${22_day_harib19a_OS} ${root_BINARY_DIR}/projects/22_day/harib19a/crack6.hrb ::crack6.hrb
  DEPENDS 22_day_harib19a_sys ${root_BINARY_DIR}/projects/22_day/harib19a/a.hrb ${root_BINARY_DIR}/projects/22_day/harib19a/hello3.hrb ${root_BINARY_DIR}/projects/22_day/harib19a/crack1.hrb ${root_BINARY_DIR}/projects/22_day/harib19a/hello.hrb ${root_BINARY_DIR}/projects/22_day/harib19a/hello2.hrb ${root_BINARY_DIR}/projects/22_day/harib19a/crack2.hrb ${root_BINARY_DIR}/projects/22_day/harib19a/crack3.hrb ${root_BINARY_DIR}/projects/22_day/harib19a/crack4.hrb ${root_BINARY_DIR}/projects/22_day/harib19a/crack5.hrb ${root_BINARY_DIR}/projects/22_day/harib19a/crack6.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(22_day_harib19a_APIO ${root_BINARY_DIR}/projects/22_day/harib19a/a_nask.o)
add_custom_command(
  OUTPUT ${22_day_harib19a_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19a/a_nask.nas ${22_day_harib19a_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19a/a_nask.nas
  COMMENT "Generating a_nask.o for 22_day_harib19a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19a/a.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19a_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19a/a.c ${22_day_harib19a_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19a/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19a/a.c ${22_day_harib19a_APIO}
  COMMENT "Generating a.hrb for 22_day_harib19a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19a/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19a_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19a/hello3.c ${22_day_harib19a_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19a/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19a/hello3.c ${22_day_harib19a_APIO}
  COMMENT "Generating hello3.hrb for 22_day_harib19a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19a/crack1.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19a_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19a/crack1.c ${22_day_harib19a_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19a/crack1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19a/crack1.c ${22_day_harib19a_APIO}
  COMMENT "Generating crack1.hrb for 22_day_harib19a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19a/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19a/hello.nas ${root_BINARY_DIR}/projects/22_day/harib19a/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19a/hello.nas
  COMMENT "Generating hello.hrb for 22_day_harib19a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19a/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19a/hello2.nas ${root_BINARY_DIR}/projects/22_day/harib19a/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19a/hello2.nas
  COMMENT "Generating hello2.hrb for 22_day_harib19a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19a/crack2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19a/crack2.nas ${root_BINARY_DIR}/projects/22_day/harib19a/crack2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19a/crack2.nas
  COMMENT "Generating crack2.hrb for 22_day_harib19a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19a/crack3.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19a/crack3.nas ${root_BINARY_DIR}/projects/22_day/harib19a/crack3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19a/crack3.nas
  COMMENT "Generating crack3.hrb for 22_day_harib19a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19a/crack4.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19a/crack4.nas ${root_BINARY_DIR}/projects/22_day/harib19a/crack4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19a/crack4.nas
  COMMENT "Generating crack4.hrb for 22_day_harib19a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19a/crack5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19a/crack5.nas ${root_BINARY_DIR}/projects/22_day/harib19a/crack5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19a/crack5.nas
  COMMENT "Generating crack5.hrb for 22_day_harib19a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19a/crack6.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19a/crack6.nas ${root_BINARY_DIR}/projects/22_day/harib19a/crack6.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19a/crack6.nas
  COMMENT "Generating crack6.hrb for 22_day_harib19a"
)
